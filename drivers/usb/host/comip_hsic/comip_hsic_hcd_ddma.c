/*==========================================================================
 * $File: //dwh/usb_iip/dev/software/otg/linux/drivers/comip_hsic_hcd_ddma.c $
 * $Revision: #8 $
 * $Date: 2010/11/29 $
 * $Change: 1636033 $
 *
 * Synopsys HS OTG Linux Software Driver and documentation (hereinafter,
 * "Software") is an Unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================== */
/** @file
 * This file contains Descriptor DMA support implementation for host mode.
 */

#include "comip_hsic_hcd.h"
#include "comip_hsic_otg_regs.h"
#include <linux/dmapool.h>

static inline uint8_t frame_list_idx(uint16_t frame)
{
    return (frame & (MAX_FRLIST_EN_NUM - 1));
}

static inline uint16_t desclist_idx_inc(uint16_t idx, uint16_t inc, uint8_t speed)
{
    return (idx + inc) &
        (((speed ==
           COMIP_HSIC_EP_SPEED_HIGH) ? MAX_DMA_DESC_NUM_HS_ISOC :
          MAX_DMA_DESC_NUM_GENERIC) - 1);
}

static inline uint16_t desclist_idx_dec(uint16_t idx, uint16_t inc, uint8_t speed)
{
    return (idx - inc) &
        (((speed ==
           COMIP_HSIC_EP_SPEED_HIGH) ? MAX_DMA_DESC_NUM_HS_ISOC :
          MAX_DMA_DESC_NUM_GENERIC) - 1);
}

static inline uint16_t max_desc_num(comip_hsic_qh_t * qh)
{
    return (((qh->ep_type == UE_ISOCHRONOUS)
         && (qh->dev_speed == COMIP_HSIC_EP_SPEED_HIGH))
        ? MAX_DMA_DESC_NUM_HS_ISOC : MAX_DMA_DESC_NUM_GENERIC);
}
static inline uint16_t frame_incr_val(comip_hsic_qh_t * qh)
{
    return ((qh->dev_speed == COMIP_HSIC_EP_SPEED_HIGH)
        ? ((qh->interval + 8 - 1) / 8)
        : qh->interval);
}

static int desc_list_alloc(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh)
{
    int retval = 0;

 //   qh->desc_list = (comip_hsic_host_dma_desc_t *)
   //     COMIP_HSIC_DMA_ALLOC_ATOMIC(sizeof(comip_hsic_host_dma_desc_t) * max_desc_num(qh),
   //           &qh->desc_list_dma);
    qh->desc_list = dma_pool_alloc(hcd->qh_pool, GFP_ATOMIC, &qh->desc_list_dma);

    if (!qh->desc_list) {
        retval = -ENOMEM;
        COMIP_HSIC_ERROR("%s: DMA descriptor list allocation failed\n", __func__);
        
    }

    memset(qh->desc_list, 0x00,
           sizeof(comip_hsic_host_dma_desc_t) * max_desc_num(qh));

    qh->n_bytes =
        (uint32_t *) kzalloc(sizeof(uint32_t) * max_desc_num(qh), GFP_ATOMIC);

    if (!qh->n_bytes) {
        retval = -ENOMEM;
        COMIP_HSIC_ERROR
            ("%s: Failed to allocate array for descriptors' size actual values\n",
             __func__);

    }
    return retval;

}

static void desc_list_free(comip_hsic_hcd_t * hcd,  comip_hsic_qh_t * qh)
{
    if (qh->desc_list) {
     //   COMIP_HSIC_DMA_FREE(max_desc_num(qh), qh->desc_list,
    //             qh->desc_list_dma);
    	dma_pool_free(hcd->qh_pool, qh->desc_list, qh->desc_list_dma);

        qh->desc_list = NULL;
    }

    if (qh->n_bytes) {
        kfree(qh->n_bytes);
        qh->n_bytes = NULL;
    }
}

static int frame_list_alloc(comip_hsic_hcd_t * hcd)
{
    int retval = 0;
    if (hcd->frame_list)
        return 0;

//    hcd->frame_list = COMIP_HSIC_DMA_ALLOC_ATOMIC(4 * MAX_FRLIST_EN_NUM,
//                    &hcd->frame_list_dma);

    hcd->frame_list = dma_pool_alloc(hcd->frame_pool, GFP_ATOMIC, &hcd->frame_list_dma);
    if (!hcd->frame_list) {
        retval = -ENOMEM;
        COMIP_HSIC_ERROR("%s: Frame List allocation failed\n", __func__);
    }

    memset(hcd->frame_list, 0x00, 4 * MAX_FRLIST_EN_NUM);

    return retval;
}

static void frame_list_free(comip_hsic_hcd_t * hcd)
{
    if (!hcd->frame_list)
        return;
    
//    COMIP_HSIC_DMA_FREE(4 * MAX_FRLIST_EN_NUM, hcd->frame_list, hcd->frame_list_dma);
	dma_pool_free(hcd->frame_pool, hcd->frame_list, hcd->frame_list_dma);
    hcd->frame_list = NULL;
}

static void per_sched_enable(comip_hsic_hcd_t * hcd, uint16_t fr_list_en)
{

    hcfg_data_t hcfg;

    hcfg.d32 = readl(&hcd->core_if->host_if->host_global_regs->hcfg);

    if (hcfg.b.perschedena) {
        /* already enabled */
        return;
    }

    COMIP_HSIC_WRITE_REG32(&hcd->core_if->host_if->host_global_regs->hflbaddr,
            hcd->frame_list_dma);

    switch (fr_list_en) {
    case 64:
        hcfg.b.frlisten = 3;
        break;
    case 32:
        hcfg.b.frlisten = 2;
        break;
    case 16:
        hcfg.b.frlisten = 1;
        break;
    case 8:
        hcfg.b.frlisten = 0;
        break;
    default:
        break;
    }

    hcfg.b.perschedena = 1;

    COMIP_HSIC_DEBUGPL(DBG_HCD, "Enabling Periodic schedule\n");
    COMIP_HSIC_WRITE_REG32(&hcd->core_if->host_if->host_global_regs->hcfg, hcfg.d32);

}

static void per_sched_disable(comip_hsic_hcd_t * hcd)
{
    hcfg_data_t hcfg;

    hcfg.d32 = readl(&hcd->core_if->host_if->host_global_regs->hcfg);
    
    if (!hcfg.b.perschedena) {
        /* already disabled */
        return;
    }
    hcfg.b.perschedena = 0;

    COMIP_HSIC_DEBUGPL(DBG_HCD, "Disabling Periodic schedule\n");
    COMIP_HSIC_WRITE_REG32(&hcd->core_if->host_if->host_global_regs->hcfg, hcfg.d32);
}

/* 
 * Activates/Deactivates FrameList entries for the channel 
 * based on endpoint servicing period.
 */
void hsic_update_frame_list(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh, uint8_t enable)
{
    uint16_t i, j, inc;
    comip_hsic_hc_t *hc = NULL;

    if (!qh->channel) {
        COMIP_HSIC_ERROR("qh->channel = %p", qh->channel);
        return;
    }

    if (!hcd) {
        COMIP_HSIC_ERROR("------hcd = %p", hcd);
        return;
    }

    if (!hcd->frame_list) {
        COMIP_HSIC_ERROR("-------hcd->frame_list = %p", hcd->frame_list);
        return;
    }

    hc = qh->channel;
    inc = frame_incr_val(qh);
    if (qh->ep_type == UE_ISOCHRONOUS)
        i = frame_list_idx(qh->sched_frame);
    else
        i = 0;

    j = i;
    do {
        if (enable)
            hcd->frame_list[j] |= (1 << hc->hc_num);
        else
            hcd->frame_list[j] &= ~(1 << hc->hc_num);
        j = (j + inc) & (MAX_FRLIST_EN_NUM - 1);
    }
    while (j != i);
    if (!enable)
        return;
    hc->schinfo = 0;
    if (qh->channel->speed == COMIP_HSIC_EP_SPEED_HIGH) {
        j = 1;
        /* TODO - check this */
        inc = (8 + qh->interval - 1) / qh->interval;
        for (i = 0; i < inc; i++) {
            hc->schinfo |= j;
            j = j << qh->interval;
        }
    } else {
        hc->schinfo = 0xff;
    }
}

static void release_channel_ddma(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh)
{
    comip_hsic_hc_t *hc = qh->channel;
    if (comip_hsic_qh_is_non_per(qh))
        hcd->non_periodic_channels--;
    else
        hsic_update_frame_list(hcd, qh, 0);

    /* 
     * The condition is added to prevent double cleanup try in case of device
     * disconnect. See channel cleanup in comip_hsic_hcd_disconnect_cb().
     */
    if (hc->qh) {
        comip_hsic_hc_cleanup(hcd->core_if, hc);
        COMIP_HSIC_CIRCLEQ_INSERT_TAIL(&hcd->free_hc_list, hc, hc_list_entry);
        hc->qh = NULL;
    }

    qh->channel = NULL;
    qh->ntd = 0;

    if (qh->desc_list) {
        memset(qh->desc_list, 0x00,
               sizeof(comip_hsic_host_dma_desc_t) * max_desc_num(qh));
    }
}

/** 
 * Initializes a QH structure's Descriptor DMA related members.
 * Allocates memory for descriptor list.
 * On first periodic QH, allocates memory for FrameList 
 * and enables periodic scheduling.
 *
 * @param hcd The HCD state structure for the COMIP OTG controller.
 * @param qh The QH to init.
 *
 * @return 0 if successful, negative error code otherwise.
 */
int comip_hsic_hcd_qh_init_ddma(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh)
{
    int retval = 0;

    if (qh->do_split) {
        COMIP_HSIC_ERROR("SPLIT Transfers are not supported in Descriptor DMA.\n");
            return -1;
        }

    retval = desc_list_alloc(hcd, qh);

    if ((retval == 0)
        && (qh->ep_type == UE_ISOCHRONOUS || qh->ep_type == UE_INTERRUPT)) {
        if (!hcd->frame_list) {
            retval = frame_list_alloc(hcd);
            /* Enable periodic schedule on first periodic QH */
            if (retval == 0)
                per_sched_enable(hcd, MAX_FRLIST_EN_NUM);
        }
    }

    qh->ntd = 0;

    return retval;
}

/** 
 * Frees descriptor list memory associated with the QH. 
 * If QH is periodic and the last, frees FrameList memory 
 * and disables periodic scheduling. 
 *
 * @param hcd The HCD state structure for the COMIP OTG controller.
 * @param qh The QH to init.
 */
void comip_hsic_hcd_qh_free_ddma(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh)
{
    desc_list_free(hcd, qh);

    /* 
     * Channel still assigned due to some reasons. 
     * Seen on Isoc URB dequeue. Channel halted but no subsequent
     * ChHalted interrupt to release the channel. Afterwards
     * when it comes here from endpoint disable routine
     * channel remains assigned.
     */
    if (qh->channel)
        release_channel_ddma(hcd, qh);

    if ((qh->ep_type == UE_ISOCHRONOUS || qh->ep_type == UE_INTERRUPT)
        && !hcd->periodic_channels && hcd->frame_list) {

        per_sched_disable(hcd);
        frame_list_free(hcd);
    }
}

static uint8_t frame_to_desc_idx(comip_hsic_qh_t * qh, uint16_t frame_idx)
{
    if (qh->dev_speed == COMIP_HSIC_EP_SPEED_HIGH) {
        /* 
         * Descriptor set(8 descriptors) index
         * which is 8-aligned.
         */
        return (frame_idx & ((MAX_DMA_DESC_NUM_HS_ISOC / 8) - 1)) * 8;
    } else {
        return (frame_idx & (MAX_DMA_DESC_NUM_GENERIC - 1));
    }
}

/* 
 * Determine starting frame for Isochronous transfer. 
 * Few frames skipped to prevent race condition with HC. 
 */
static uint8_t calc_starting_frame(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh,
    uint8_t * skip_frames)
{
    uint16_t frame = 0;
    hcd->frame_number = comip_hsic_hcd_get_frame_number(hcd);
    
    /* sched_frame is always frame number(not uFrame) both in FS and HS !! */
    
    /* 
     * skip_frames is used to limit activated descriptors number
     * to avoid the situation when HC services the last activated
     * descriptor firstly.
     * Example for FS:
     * Current frame is 1, scheduled frame is 3. Since HC always fetches the descriptor
     * corresponding to curr_frame+1, the descriptor corresponding to frame 2
     * will be fetched. If the number of descriptors is max=64 (or greather) the
     * list will be fully programmed with Active descriptors and it is possible
     * case(rare) that the latest descriptor(considering rollback) corresponding
     * to frame 2 will be serviced first. HS case is more probable because, in fact,
     * up to 11 uframes(16 in the code) may be skipped.
     */
    if (qh->dev_speed == COMIP_HSIC_EP_SPEED_HIGH) {
        /* 
         * Consider uframe counter also, to start xfer asap.
         * If half of the frame elapsed skip 2 frames otherwise
         * just 1 frame. 
         * Starting descriptor index must be 8-aligned, so
         * if the current frame is near to complete the next one
         * is skipped as well.
         */

        if (comip_hsic_micro_frame_num(hcd->frame_number) >= 5) {
            *skip_frames = 2 * 8;
            frame = comip_hsic_frame_num_inc(hcd->frame_number, *skip_frames);
        } else {
            *skip_frames = 1 * 8;
            frame = comip_hsic_frame_num_inc(hcd->frame_number, *skip_frames);
        }

        frame = comip_hsic_full_frame_num(frame);
    } else {
        /* 
         * Two frames are skipped for FS - the current and the next.
         * But for descriptor programming, 1 frame(descriptor) is enough,
         * see example above.
         */
        *skip_frames = 1;
        frame = comip_hsic_frame_num_inc(hcd->frame_number, 2);
    }

    return frame;
}

/* 
 * Calculate initial descriptor index for isochronous transfer
 * based on scheduled frame. 
 */
static uint8_t recalc_initial_desc_idx(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh)
{
    uint16_t frame = 0, fr_idx, fr_idx_tmp;
    uint8_t skip_frames = 0;
    /* 
     * With current ISOC processing algorithm the channel is being
     * released when no more QTDs in the list(qh->ntd == 0).
     * Thus this function is called only when qh->ntd == 0 and qh->channel == 0. 
     *
     * So qh->channel != NULL branch is not used and just not removed from the
     * source file. It is required for another possible approach which is,
     * do not disable and release the channel when ISOC session completed, 
     * just move QH to inactive schedule until new QTD arrives. 
     * On new QTD, the QH moved back to 'ready' schedule,
     * starting frame and therefore starting desc_index are recalculated.
     * In this case channel is released only on ep_disable.
     */

    /* Calculate starting descriptor index. For INTERRUPT endpoint it is always 0. */
    if (qh->channel) {
        frame = calc_starting_frame(hcd, qh, &skip_frames);
        /* 
         * Calculate initial descriptor index based on FrameList current bitmap
         * and servicing period.
         */
        fr_idx_tmp = frame_list_idx(frame);
        fr_idx =
            (MAX_FRLIST_EN_NUM + frame_list_idx(qh->sched_frame) -
             fr_idx_tmp)
            % frame_incr_val(qh);
        fr_idx = (fr_idx + fr_idx_tmp) % MAX_FRLIST_EN_NUM;
    } else {
        qh->sched_frame = calc_starting_frame(hcd, qh, &skip_frames);
        fr_idx = frame_list_idx(qh->sched_frame);
    }

    qh->td_first = qh->td_last = frame_to_desc_idx(qh, fr_idx);

    return skip_frames;
}

#define ISOC_URB_GIVEBACK_ASAP

#define MAX_ISOC_XFER_SIZE_FS 1023
#define MAX_ISOC_XFER_SIZE_HS 3072
#define DESCNUM_THRESHOLD 4

static void init_isoc_dma_desc(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh,
    uint8_t skip_frames)
{
    struct comip_hsic_hcd_iso_packet_desc *frame_desc;
    comip_hsic_qtd_t *qtd;
    comip_hsic_host_dma_desc_t *dma_desc;
    uint16_t idx, inc, n_desc, ntd_max, max_xfer_size;

    idx = qh->td_last;
    inc = qh->interval;
    n_desc = 0;

    ntd_max = (max_desc_num(qh) + qh->interval - 1) / qh->interval;
    if (skip_frames && !qh->channel)
        ntd_max = ntd_max - skip_frames / qh->interval;

    max_xfer_size =
        (qh->dev_speed ==
         COMIP_HSIC_EP_SPEED_HIGH) ? MAX_ISOC_XFER_SIZE_HS :
        MAX_ISOC_XFER_SIZE_FS;

    COMIP_HSIC_CIRCLEQ_FOREACH(qtd, &qh->qtd_list, qtd_list_entry) {
        while ((qh->ntd < ntd_max)
               && (qtd->isoc_frame_index_last <
               qtd->urb->packet_count)) {

            dma_desc = &qh->desc_list[idx];
            memset(dma_desc, 0x00, sizeof(comip_hsic_host_dma_desc_t));

            frame_desc = &qtd->urb->iso_descs[qtd->isoc_frame_index_last];

            if (frame_desc->length > max_xfer_size)
                qh->n_bytes[idx] = max_xfer_size;
            else
                qh->n_bytes[idx] = frame_desc->length;
            dma_desc->status.b_isoc.n_bytes = qh->n_bytes[idx];
            dma_desc->status.b_isoc.a = 1;

            dma_desc->buf = qtd->urb->dma + frame_desc->offset;

            qh->ntd++;

            qtd->isoc_frame_index_last++;

#ifdef  ISOC_URB_GIVEBACK_ASAP
            /* 
             * Set IOC for each descriptor corresponding to the 
             * last frame of the URB.
             */
            if (qtd->isoc_frame_index_last ==
                qtd->urb->packet_count)
                dma_desc->status.b_isoc.ioc = 1;

#endif
            idx = desclist_idx_inc(idx, inc, qh->dev_speed);
            n_desc++;

        }
        qtd->in_process = 1;
    }

    qh->td_last = idx;

#ifdef  ISOC_URB_GIVEBACK_ASAP
    /* Set IOC for the last descriptor if descriptor list is full */
    if (qh->ntd == ntd_max) {
        idx = desclist_idx_dec(qh->td_last, inc, qh->dev_speed);
        qh->desc_list[idx].status.b_isoc.ioc = 1;
    }
#else
    /* 
     * Set IOC bit only for one descriptor. 
     * Always try to be ahead of HW processing,
     * i.e. on IOC generation driver activates next descriptors but
     * core continues to process descriptors followed the one with IOC set.
     */

    if (n_desc > DESCNUM_THRESHOLD) {
        /* 
         * Move IOC "up". Required even if there is only one QTD 
         * in the list, cause QTDs migth continue to be queued,
         * but during the activation it was only one queued.
         * Actually more than one QTD might be in the list if this function called 
         * from XferCompletion - QTDs was queued during HW processing of the previous
         * descriptor chunk.
         */
        idx = comip_hsic_desclist_idx_dec(idx, inc * ((qh->ntd + 1) / 2), qh->dev_speed);
    } else {
        /* 
         * Set the IOC for the latest descriptor
         * if either number of descriptor is not greather than threshold
         * or no more new descriptors activated.
         */
        idx = comip_hsic_desclist_idx_dec(qh->td_last, inc, qh->dev_speed);
    }

    qh->desc_list[idx].status.b_isoc.ioc = 1;
#endif
}

static void init_non_isoc_dma_desc(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh)
{

    comip_hsic_hc_t *hc;
    comip_hsic_host_dma_desc_t *dma_desc;
    comip_hsic_qtd_t *qtd;
    int num_packets, len, n_desc = 0;

    hc = qh->channel;

    /* 
     * Start with hc->xfer_buff initialized in 
     * assign_and_init_hc(), then if SG transfer consists of multiple URBs,
     * this pointer re-assigned to the buffer of the currently processed QTD.
     * For non-SG request there is always one QTD active.
     */

    COMIP_HSIC_CIRCLEQ_FOREACH(qtd, &qh->qtd_list, qtd_list_entry) {

        if (n_desc) {
            /* SG request - more than 1 QTDs */
            hc->xfer_buff = (uint8_t *)qtd->urb->dma + qtd->urb->actual_length;
            hc->xfer_len = qtd->urb->length - qtd->urb->actual_length;
        }

        qtd->n_desc = 0;

        do {
            dma_desc = &qh->desc_list[n_desc];
            len = hc->xfer_len;

            if (len > MAX_DMA_DESC_SIZE)
                len = MAX_DMA_DESC_SIZE - hc->max_packet + 1;

            if (hc->ep_is_in) {
                if (len > 0) {
                    num_packets = (len + hc->max_packet - 1) / hc->max_packet;
                } else {
                    /* Need 1 packet for transfer length of 0. */
                    num_packets = 1;
                }
                /* Always program an integral # of max packets for IN transfers. */
                len = num_packets * hc->max_packet;
            }

            dma_desc->status.b.n_bytes = len;

            qh->n_bytes[n_desc] = len;

            if ((qh->ep_type == UE_CONTROL)
                && (qtd->control_phase == COMIP_HSIC_CONTROL_SETUP))
                dma_desc->status.b.sup = 1; /* Setup Packet */

            dma_desc->status.b.a = 1;   /* Active descriptor */

            dma_desc->buf =
                ((uint32_t)hc->xfer_buff & 0xffffffff);

            /* 
             * Last descriptor(or single) of IN transfer 
             * with actual size less than MaxPacket.
             */
            if (len > hc->xfer_len) {
                hc->xfer_len = 0;
            } else {
                hc->xfer_buff += len;
                hc->xfer_len -= len;
            }

            qtd->n_desc++;
            n_desc++;
        }
        while ((hc->xfer_len > 0) && (n_desc != MAX_DMA_DESC_NUM_GENERIC));
        

        qtd->in_process = 1;

        if (n_desc == MAX_DMA_DESC_NUM_GENERIC)
            break;
    }

    if (n_desc) {
        /* Request Transfer Complete interrupt for the last descriptor */
        qh->desc_list[n_desc - 1].status.b.ioc = 1;
        /* End of List indicator */
        qh->desc_list[n_desc - 1].status.b.eol = 1;

        hc->ntd = n_desc;
    }
}

/** 
 * For Control and Bulk endpoints initializes descriptor list
 * and starts the transfer.
 *
 * For Interrupt and Isochronous endpoints initializes descriptor list
 * then updates FrameList, marking appropriate entries as active.
 * In case of Isochronous, the starting descriptor index is calculated based
 * on the scheduled frame, but only on the first transfer descriptor within a session.
 * Then starts the transfer via enabling the channel. 
 * For Isochronous endpoint the channel is not halted on XferComplete 
 * interrupt so remains assigned to the endpoint(QH) until session is done.
 *
 * @param hcd The HCD state structure for the COMIP OTG controller.
 * @param qh The QH to init.
 *
 * @return 0 if successful, negative error code otherwise.
 */
void comip_hsic_hcd_start_xfer_ddma(comip_hsic_hcd_t * hcd, comip_hsic_qh_t * qh)
{
    /* Channel is already assigned */
    comip_hsic_hc_t *hc = qh->channel;
    uint8_t skip_frames = 0;

    switch (hc->ep_type) {
    case COMIP_HSIC_EP_TYPE_CONTROL:
    case COMIP_HSIC_EP_TYPE_BULK:
        init_non_isoc_dma_desc(hcd, qh);

        comip_hsic_hc_start_transfer_ddma(hcd->core_if, hc);
        break;
    case COMIP_HSIC_EP_TYPE_INTR:
        init_non_isoc_dma_desc(hcd, qh);

        hsic_update_frame_list(hcd, qh, 1);

        comip_hsic_hc_start_transfer_ddma(hcd->core_if, hc);
        break;
    case COMIP_HSIC_EP_TYPE_ISOC:

        if (!qh->ntd)
            skip_frames = recalc_initial_desc_idx(hcd, qh);

        init_isoc_dma_desc(hcd, qh, skip_frames);

        if (!hc->xfer_started) {

            hsic_update_frame_list(hcd, qh, 1);

            /* 
             * Always set to max, instead of actual size.
             * Otherwise ntd will be changed with 
             * channel being enabled. Not recommended.
             *
             */
            hc->ntd = max_desc_num(qh);
            /* Enable channel only once for ISOC */
            comip_hsic_hc_start_transfer_ddma(hcd->core_if, hc);
        }

        break;
    default:

        break;
    }
}

static void complete_isoc_xfer_ddma(comip_hsic_hcd_t * hcd,
    comip_hsic_hc_t * hc, comip_hsic_hc_regs_t * hc_regs,
    comip_hsic_halt_status_e halt_status)
{
    struct comip_hsic_hcd_iso_packet_desc *frame_desc;
    comip_hsic_qtd_t *qtd, *qtd_tmp;
    comip_hsic_qh_t *qh;
    comip_hsic_host_dma_desc_t *dma_desc;
    uint16_t idx, remain;
    uint8_t urb_compl;

    qh = hc->qh;
    idx = qh->td_first;

    if (hc->halt_status == COMIP_HSIC_HC_XFER_URB_DEQUEUE) {
        COMIP_HSIC_CIRCLEQ_FOREACH_SAFE(qtd, qtd_tmp, &hc->qh->qtd_list, qtd_list_entry)
            qtd->in_process = 0;
        return;
    } else if ((halt_status == COMIP_HSIC_HC_XFER_AHB_ERR) ||
           (halt_status == COMIP_HSIC_HC_XFER_BABBLE_ERR)) {
        /* 
         * Channel is halted in these error cases.
         * Considered as serious issues.
         * Complete all URBs marking all frames as failed, 
         * irrespective whether some of the descriptors(frames) succeeded or no.
         * Pass error code to completion routine as well, to
         * update urb->status, some of class drivers might use it to stop
         * queing transfer requests.
         */
        int err = (halt_status == COMIP_HSIC_HC_XFER_AHB_ERR)
            ? (-EIO)
            : (-EOVERFLOW);
                        
        COMIP_HSIC_CIRCLEQ_FOREACH_SAFE(qtd, qtd_tmp, &hc->qh->qtd_list, qtd_list_entry) {
            for (idx = 0; idx < qtd->urb->packet_count; idx++) {
                frame_desc = &qtd->urb->iso_descs[idx];
                frame_desc->status = err;
            }
            hcd->fops->complete(hcd, qtd->urb->priv, qtd->urb, err);
            comip_hsic_hcd_qtd_remove_and_free(hcd, qtd, qh);
        }
        return;
    }

    COMIP_HSIC_CIRCLEQ_FOREACH_SAFE(qtd, qtd_tmp, &hc->qh->qtd_list, qtd_list_entry) {

        if (!qtd->in_process)
            break;

        urb_compl = 0;

        do {

            dma_desc = &qh->desc_list[idx];
            
            frame_desc = &qtd->urb->iso_descs[qtd->isoc_frame_index];
            remain = hc->ep_is_in ? dma_desc->status.b_isoc.n_bytes : 0;

            if (dma_desc->status.b_isoc.sts == DMA_DESC_STS_PKTERR) {
                /* 
                 * XactError or, unable to complete all the transactions 
                 * in the scheduled micro-frame/frame, 
                 * both indicated by DMA_DESC_STS_PKTERR.
                 */
                qtd->urb->error_count++;
                frame_desc->actual_length = qh->n_bytes[idx] - remain;
                frame_desc->status = -EPROTO;
            } else {
                /* Success */
                                
                frame_desc->actual_length = qh->n_bytes[idx] - remain;
                frame_desc->status = 0;
            }

            if (++qtd->isoc_frame_index == qtd->urb->packet_count) {
                /*
                 * urb->status is not used for isoc transfers here.
                 * The individual frame_desc status are used instead.
                 */

                hcd->fops->complete(hcd, qtd->urb->priv, qtd->urb, 0);
                comip_hsic_hcd_qtd_remove_and_free(hcd, qtd, qh);

                /* 
                 * This check is necessary because urb_dequeue can be called 
                 * from urb complete callback(sound driver example).
                 * All pending URBs are dequeued there, so no need for
                 * further processing.
                 */
                if (hc->halt_status == COMIP_HSIC_HC_XFER_URB_DEQUEUE) {    
                    return;
                }

                urb_compl = 1;

            }

            qh->ntd--;

            /* Stop if IOC requested descriptor reached */
            if (dma_desc->status.b_isoc.ioc) {
                idx = desclist_idx_inc(idx, qh->interval, hc->speed);   
                goto stop_scan;
            }

            idx = desclist_idx_inc(idx, qh->interval, hc->speed);

            if (urb_compl)
                break;
        }
        while (idx != qh->td_first);
    }
stop_scan:
    qh->td_first = idx;
}

uint8_t hsic_update_non_isoc_urb_state_ddma(comip_hsic_hcd_t * hcd,
    comip_hsic_hc_t * hc, comip_hsic_qtd_t * qtd, comip_hsic_host_dma_desc_t * dma_desc,
    comip_hsic_halt_status_e halt_status, uint32_t n_bytes, uint8_t * xfer_done)
{

    uint16_t remain = hc->ep_is_in ? dma_desc->status.b.n_bytes : 0;
    comip_hsic_hcd_urb_t *urb = qtd->urb;

    if (halt_status == COMIP_HSIC_HC_XFER_AHB_ERR) {
        urb->status = -EIO;
        return 1;
    }
    if (dma_desc->status.b.sts == DMA_DESC_STS_PKTERR) {
        switch (halt_status) {
        case COMIP_HSIC_HC_XFER_STALL:
            urb->status = -EPIPE;
            break;
        case COMIP_HSIC_HC_XFER_BABBLE_ERR:
            urb->status = -EOVERFLOW;
            break;
        case COMIP_HSIC_HC_XFER_XACT_ERR:
            urb->status = -EPROTO;
            break;
        default:    
            COMIP_HSIC_ERROR("%s: Unhandled descriptor error status (%d)\n", __func__,
                  halt_status);
            break;
        }
        return 1;
    }

    if (dma_desc->status.b.a == 1) {
        COMIP_HSIC_DEBUGPL(DBG_HCDV,
                "Active descriptor encountered on channel %d\n",
                hc->hc_num);
        return 0;
    }

    if (hc->ep_type == COMIP_HSIC_EP_TYPE_CONTROL) {
        if (qtd->control_phase == COMIP_HSIC_CONTROL_DATA) {
            urb->actual_length += n_bytes - remain;
            if (remain || urb->actual_length == urb->length) {
                /* 
                 * For Control Data stage do not set urb->status=0 to prevent
                 * URB callback. Set it when Status phase done. See below.
                 */
                *xfer_done = 1;
            }

        } else if (qtd->control_phase == COMIP_HSIC_CONTROL_STATUS) {
            urb->status = 0;
            *xfer_done = 1;
        }
        /* No handling for SETUP stage */
    } else {
        /* BULK and INTR */
        urb->actual_length += n_bytes - remain;
        if (remain || urb->actual_length == urb->length) {
            urb->status = 0;
            *xfer_done = 1;
        }
    }

    return 0;
}

static void complete_non_isoc_xfer_ddma(comip_hsic_hcd_t * hcd,
    comip_hsic_hc_t * hc, comip_hsic_hc_regs_t * hc_regs, 
    comip_hsic_halt_status_e halt_status)
{
    comip_hsic_hcd_urb_t *urb = NULL;
    comip_hsic_qtd_t *qtd, *qtd_tmp;
    comip_hsic_qh_t *qh;
    comip_hsic_host_dma_desc_t *dma_desc;
    uint32_t n_bytes, n_desc, i;
    uint8_t failed = 0, xfer_done;
    uint8_t qtd_n_desc;

    n_desc = 0;

    qh = hc->qh;

    if (hc->halt_status == COMIP_HSIC_HC_XFER_URB_DEQUEUE) {
        COMIP_HSIC_CIRCLEQ_FOREACH_SAFE(qtd, qtd_tmp, &hc->qh->qtd_list, qtd_list_entry) {
            qtd->in_process = 0;
        }
        return;
    }

    COMIP_HSIC_CIRCLEQ_FOREACH_SAFE(qtd, qtd_tmp, &qh->qtd_list, qtd_list_entry) {
        urb = qtd->urb;
        n_bytes = 0;
        qtd_n_desc = qtd->n_desc;
        
        for (i = 0; i < qtd_n_desc; i++) {
            xfer_done = 0;
            dma_desc = &qh->desc_list[n_desc];

            n_bytes = qh->n_bytes[n_desc];

            failed =
                hsic_update_non_isoc_urb_state_ddma(hcd, hc, qtd,
                               dma_desc,
                               halt_status, n_bytes,
                               &xfer_done);

            if (failed
                || (xfer_done
                && (urb->status != -EINPROGRESS))) {

                hcd->fops->complete(hcd, urb->priv, urb,
                            urb->status);
                comip_hsic_hcd_qtd_remove_and_free(hcd, qtd, qh);

                if (failed)
                    goto stop_scan;
            } else if (qh->ep_type == UE_CONTROL) {
                if (qtd->control_phase == COMIP_HSIC_CONTROL_SETUP) {
                    if (urb->length > 0) {
                        qtd->control_phase = COMIP_HSIC_CONTROL_DATA;
                    } else {
                        qtd->control_phase = COMIP_HSIC_CONTROL_STATUS;
                    }
                    COMIP_HSIC_DEBUGPL(DBG_HCDV, "  Control setup transaction done\n");
                } else if (qtd->control_phase == COMIP_HSIC_CONTROL_DATA) {
                    if (xfer_done) {
                        qtd->control_phase = COMIP_HSIC_CONTROL_STATUS;
                        COMIP_HSIC_DEBUGPL(DBG_HCDV, "  Control data transfer done\n");
                    } else if (i + 1 == qtd->n_desc) {
                        /* 
                         * Last descriptor for Control data stage which is
                         * not completed yet.
                         */
                        comip_hsic_hcd_save_data_toggle(hc, hc_regs, qtd);
                    }
                }
            }

            n_desc++;
        }

    }

stop_scan:

    if (qh->ep_type != UE_CONTROL) {
        /* 
         * Resetting the data toggle for bulk
         * and interrupt endpoints in case of stall. See handle_hc_stall_intr() 
         */
        if (halt_status == COMIP_HSIC_HC_XFER_STALL)
            qh->data_toggle = COMIP_HSIC_HC_PID_DATA0;
        else
            comip_hsic_hcd_save_data_toggle(hc, hc_regs, qtd);
    }

    if (halt_status == COMIP_HSIC_HC_XFER_COMPLETE) {
        hcint_data_t hcint;
        hcint.d32 = readl(&hc_regs->hcint);
        if (hcint.b.nyet) {
            /*
             * Got a NYET on the last transaction of the transfer. It
             * means that the endpoint should be in the PING state at the
             * beginning of the next transfer.
             */
            qh->ping_state = 1;
            clear_hc_int(hc_regs, nyet);
        }

    }

}

/**
 * This function is called from interrupt handlers.
 * Scans the descriptor list, updates URB's status and
 * calls completion routine for the URB if it's done.
 * Releases the channel to be used by other transfers.
 * In case of Isochronous endpoint the channel is not halted until 
 * the end of the session, i.e. QTD list is empty.
 * If periodic channel released the FrameList is updated accordingly.
 *
 * Calls transaction selection routines to activate pending transfers.
 *
 * @param hcd The HCD state structure for the COMIP OTG controller.
 * @param hc Host channel, the transfer is completed on.
 * @param hc_regs Host channel registers.
 * @param halt_status Reason the channel is being halted, 
 *            or just XferComplete for isochronous transfer
 */
void comip_hsic_hcd_complete_xfer_ddma(comip_hsic_hcd_t * hcd,
    comip_hsic_hc_t * hc, comip_hsic_hc_regs_t * hc_regs,
    comip_hsic_halt_status_e halt_status)
{
    uint8_t continue_isoc_xfer = 0;
    comip_hsic_transaction_type_e tr_type;
    comip_hsic_qh_t *qh = hc->qh;

    if (hc->ep_type == COMIP_HSIC_EP_TYPE_ISOC) {

        complete_isoc_xfer_ddma(hcd, hc, hc_regs, halt_status);

        /* Release the channel if halted or session completed */
        if (halt_status != COMIP_HSIC_HC_XFER_COMPLETE ||
            COMIP_HSIC_CIRCLEQ_EMPTY(&qh->qtd_list)) {

            /* Halt the channel if session completed */
            if (halt_status == COMIP_HSIC_HC_XFER_COMPLETE) {
                comip_hsic_hc_halt(hcd->core_if, hc, halt_status);
            }

            release_channel_ddma(hcd, qh);
            comip_hsic_hcd_qh_remove(hcd, qh);
        } else {
            /* Keep in assigned schedule to continue transfer */
            COMIP_HSIC_LIST_MOVE_HEAD(&hcd->periodic_sched_assigned,
                       &qh->qh_list_entry);
            continue_isoc_xfer = 1;

        }
        /** @todo Consider the case when period exceeds FrameList size.
         *  Frame Rollover interrupt should be used. 
         */
    } else {
        /* Scan descriptor list to complete the URB(s), then release the channel */
        complete_non_isoc_xfer_ddma(hcd, hc, hc_regs, halt_status);

        release_channel_ddma(hcd, qh);
        comip_hsic_hcd_qh_remove(hcd, qh);

        if (!COMIP_HSIC_CIRCLEQ_EMPTY(&qh->qtd_list)) {
            /* Add back to inactive non-periodic schedule on normal completion */
            comip_hsic_hcd_qh_add(hcd, qh);
        }

    }
    tr_type = comip_hsic_hcd_select_transactions(hcd);
    if (tr_type != COMIP_HSIC_TRANSACTION_NONE || continue_isoc_xfer) {
        if (continue_isoc_xfer) {
            if (tr_type == COMIP_HSIC_TRANSACTION_NONE) {
                tr_type = COMIP_HSIC_TRANSACTION_PERIODIC;
            } else if (tr_type == COMIP_HSIC_TRANSACTION_NON_PERIODIC) {
                tr_type = COMIP_HSIC_TRANSACTION_ALL;
            }
        }
        comip_hsic_hcd_queue_transactions(hcd, tr_type);
    }
}
