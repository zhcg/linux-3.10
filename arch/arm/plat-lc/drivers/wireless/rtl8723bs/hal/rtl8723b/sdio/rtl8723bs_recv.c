/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#define _RTL8723BS_RECV_C_

#include <drv_conf.h>

#if defined (PLATFORM_LINUX) && defined (PLATFORM_WINDOWS)
#error "Shall be Linux or Windows, but not both!\n"
#endif

#include <drv_types.h>
#include <recv_osdep.h>
#include <rtl8723b_hal.h>


static void update_recvframe_attrib_8723b(
	PADAPTER padapter,
	union recv_frame *precvframe,
	struct recv_stat *prxstat)
{
	struct rx_pkt_attrib	*pattrib;
	struct recv_stat	report;
	PRXREPORT prxreport = (PRXREPORT)&report;

	report.rxdw0 = le32_to_cpu(prxstat->rxdw0);
	report.rxdw1 = le32_to_cpu(prxstat->rxdw1);
	report.rxdw2 = le32_to_cpu(prxstat->rxdw2);
	report.rxdw3 = le32_to_cpu(prxstat->rxdw3);
	report.rxdw4 = le32_to_cpu(prxstat->rxdw4);
	report.rxdw5 = le32_to_cpu(prxstat->rxdw5);

	pattrib = &precvframe->u.hdr.attrib;
	_rtw_memset(pattrib, 0, sizeof(struct rx_pkt_attrib));

	// update rx report to recv_frame attribute
	pattrib->pkt_rpt_type = ((report.rxdw2 >> 28) & 0x1)?C2H_PACKET:NORMAL_RX;

	//DBG_871X("%s(): pkt_rpt_type=%d\n", __func__, pattrib->pkt_rpt_type);
	if(pattrib->pkt_rpt_type == NORMAL_RX)//Normal rx packet
	{
		// update rx report to recv_frame attribute
		pattrib->pkt_len = (u16)prxreport->pktlen;
		pattrib->drvinfo_sz = (u8)(prxreport->drvinfosize << 3);
		pattrib->physt = (u8)prxreport->physt;

		pattrib->crc_err = (u8)prxreport->crc32;
		pattrib->icv_err = (u8)prxreport->icverr;

		pattrib->bdecrypted = (u8)(prxreport->swdec ? 0 : 1);
		pattrib->encrypt = (u8)prxreport->security;

		pattrib->qos = (u8)prxreport->qos;
		pattrib->priority = (u8)prxreport->tid;

		pattrib->amsdu = (u8)prxreport->amsdu;

		pattrib->seq_num = (u16)prxreport->seq;
		pattrib->frag_num = (u8)prxreport->frag;
		pattrib->mfrag = (u8)prxreport->mf;
		pattrib->mdata = (u8)prxreport->md;

		pattrib->mcs_rate = (u8)prxreport->rxrate;
	}
	else
	{
		pattrib->pkt_len = (u16)prxreport->pktlen;
	}
}

/*
 * Notice:
 *	Before calling this function,
 *	precvframe->u.hdr.rx_data should be ready!
 */
void update_recvframe_phyinfo(
	union recv_frame	*precvframe,
	struct phy_stat *pphy_status)
{
	PADAPTER 			padapter= precvframe->u.hdr.adapter;
	struct rx_pkt_attrib	*pattrib = &precvframe->u.hdr.attrib;
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(padapter);
	PODM_PHY_INFO_T 	pPHYInfo = (PODM_PHY_INFO_T)(&pattrib->phy_info);

	u8			*wlanhdr;
	ODM_PACKET_INFO_T	pkt_info;
	u8 *sa;
	//_irqL		irqL;
	struct sta_priv *pstapriv;
	struct sta_info *psta;

	pkt_info.bPacketMatchBSSID =_FALSE;
	pkt_info.bPacketToSelf = _FALSE;
	pkt_info.bPacketBeacon = _FALSE;


	wlanhdr = get_recvframe_data(precvframe);

	pkt_info.bPacketMatchBSSID = ((!IsFrameTypeCtrl(wlanhdr)) &&
		!pattrib->icv_err && !pattrib->crc_err &&
		_rtw_memcmp(get_hdr_bssid(wlanhdr), get_bssid(&padapter->mlmepriv), ETH_ALEN));

	pkt_info.bPacketToSelf = pkt_info.bPacketMatchBSSID && (_rtw_memcmp(get_da(wlanhdr), myid(&padapter->eeprompriv), ETH_ALEN));

	pkt_info.bPacketBeacon = pkt_info.bPacketMatchBSSID && (GetFrameSubType(wlanhdr) == WIFI_BEACON);

	if(pkt_info.bPacketBeacon){
		if(check_fwstate(&padapter->mlmepriv, WIFI_STATION_STATE) == _TRUE){
			sa = padapter->mlmepriv.cur_network.network.MacAddress;
			#if 0
			{
				DBG_8192C("==> rx beacon from AP[%02x:%02x:%02x:%02x:%02x:%02x]\n",
					sa[0],sa[1],sa[2],sa[3],sa[4],sa[5]);
			}
			#endif
		}
		//to do Ad-hoc
	}
	else{
		sa = get_sa(wlanhdr);
	}

	pkt_info.StationID = 0xFF;

	pstapriv = &padapter->stapriv;
	psta = rtw_get_stainfo(pstapriv, sa);
	if (psta)
	{
      		pkt_info.StationID = psta->mac_id;
		//DBG_8192C("%s ==> StationID(%d)\n",__FUNCTION__,pkt_info.StationID);
	}
	pkt_info.Rate = pattrib->mcs_rate;

	#ifdef CONFIG_CONCURRENT_MODE
	//get Primary adapter's odmpriv
	if(padapter->adapter_type > PRIMARY_ADAPTER){
		pHalData = GET_HAL_DATA(padapter->pbuddy_adapter);
	}
	#endif
	//rtl8723b_query_rx_phy_status(precvframe, pphy_status);
	//_enter_critical_bh(&pHalData->odm_stainfo_lock, &irqL);
	ODM_PhyStatusQuery(&pHalData->odmpriv,pPHYInfo,(u8 *)pphy_status,&(pkt_info));
	//_exit_critical_bh(&pHalData->odm_stainfo_lock, &irqL);
	precvframe->u.hdr.psta = NULL;
	if (pkt_info.bPacketMatchBSSID &&
		(check_fwstate(&padapter->mlmepriv, WIFI_AP_STATE) == _TRUE))
	{
		if (psta)
		{
			precvframe->u.hdr.psta = psta;
			rtl8723b_process_phy_info(padapter, precvframe);
              }
	}
	else if (pkt_info.bPacketToSelf || pkt_info.bPacketBeacon)
	{
		if (check_fwstate(&padapter->mlmepriv, WIFI_ADHOC_STATE|WIFI_ADHOC_MASTER_STATE) == _TRUE)
		{
			if (psta)
			{
				precvframe->u.hdr.psta = psta;
			}
		}
		rtl8723b_process_phy_info(padapter, precvframe);
#ifdef CONFIG_BT_COEXIST
		if(pkt_info.bPacketToSelf){
			u8 isCCKrate = 0;
			isCCKrate = ((pkt_info.Rate >= DESC92C_RATE1M ) && (pkt_info.Rate <= DESC92C_RATE5_5M ))?TRUE :FALSE;
			//DBG_871X("RXX rate:%x,len:%d,RSSI:%d,isCCKrate:%d\n",pkt_info.Rate,precvframe->u.hdr.len,padapter->recvpriv.rssi,isCCKrate);
			if(isCCKrate && pHalData->bBtScoBusy){
				pHalData->bupdate0x778 = _TRUE;
			}
			else{
				pHalData->bupdate0x778 = _FALSE;
			}
		}
#endif
	}
}

#ifdef CONFIG_C2H_PACKET_EN
static void rtl8723bs_c2h_packet_handler(PADAPTER padapter, u8 *pbuf, u16 length)
{
	u8 *tmpBuf=NULL;

	if(length == 0)
		return;

	DBG_871X("+%s() length=%d\n", __func__, length);

	tmpBuf = rtw_zmalloc(length);
	if (tmpBuf == NULL)
		return;

	_rtw_memcpy(tmpBuf, pbuf, length);

	rtw_c2h_packet_wk_cmd(padapter, tmpBuf, length);

	DBG_871X("-%s()\n", __func__);

	return;
}
#endif

#ifdef CONFIG_SDIO_RX_COPY
static s32 pre_recv_entry(union recv_frame *precvframe, struct recv_buf *precvbuf, struct phy_stat *pphy_status)
{
	s32 ret=_SUCCESS;
#ifdef CONFIG_CONCURRENT_MODE
	u8 *primary_myid, *secondary_myid, *paddr1;
	union recv_frame	*precvframe_if2 = NULL;
	_adapter *primary_padapter = precvframe->u.hdr.adapter;
	_adapter *secondary_padapter = primary_padapter->pbuddy_adapter;
	struct recv_priv *precvpriv = &primary_padapter->recvpriv;
	_queue *pfree_recv_queue = &precvpriv->free_recv_queue;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(primary_padapter);

	if(!secondary_padapter)
		return ret;

	paddr1 = GetAddr1Ptr(precvframe->u.hdr.rx_data);

	if(IS_MCAST(paddr1) == _FALSE)//unicast packets
	{
		//primary_myid = myid(&primary_padapter->eeprompriv);
		secondary_myid = myid(&secondary_padapter->eeprompriv);

		if(_rtw_memcmp(paddr1, secondary_myid, ETH_ALEN))
		{
			//change to secondary interface
			precvframe->u.hdr.adapter = secondary_padapter;
		}

		//ret = recv_entry(precvframe);

	}
	else // Handle BC/MC Packets
	{
		//clone/copy to if2
		_pkt	 *pkt_copy = NULL;
		struct rx_pkt_attrib *pattrib = NULL;

		precvframe_if2 = rtw_alloc_recvframe(pfree_recv_queue);

		if(!precvframe_if2)
			return _FAIL;

		precvframe_if2->u.hdr.adapter = secondary_padapter;
		_rtw_memcpy(&precvframe_if2->u.hdr.attrib, &precvframe->u.hdr.attrib, sizeof(struct rx_pkt_attrib));
		pattrib = &precvframe_if2->u.hdr.attrib;

		//driver need to set skb len for skb_copy().
		//If skb->len is zero, skb_copy() will not copy data from original skb.
		skb_put(precvframe->u.hdr.pkt, pattrib->pkt_len);

		pkt_copy = skb_copy( precvframe->u.hdr.pkt, GFP_ATOMIC);
		if (pkt_copy == NULL)
		{
			if((pattrib->mfrag == 1)&&(pattrib->frag_num == 0))
			{
				DBG_8192C("pre_recv_entry(): skb_copy fail , drop frag frame \n");
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				return ret;
			}

			pkt_copy = skb_clone( precvframe->u.hdr.pkt, GFP_ATOMIC);
			if(pkt_copy == NULL)
			{
				DBG_8192C("pre_recv_entry(): skb_clone fail , drop frame\n");
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				return ret;
			}
		}

		pkt_copy->dev = secondary_padapter->pnetdev;

		precvframe_if2->u.hdr.pkt = pkt_copy;
		precvframe_if2->u.hdr.rx_head = pkt_copy->head;
		precvframe_if2->u.hdr.rx_data = pkt_copy->data;
		precvframe_if2->u.hdr.rx_tail = skb_tail_pointer(pkt_copy);
		precvframe_if2->u.hdr.rx_end = skb_end_pointer(pkt_copy);
		precvframe_if2->u.hdr.len = pkt_copy->len;

		//recvframe_put(precvframe_if2, pattrib->pkt_len);

		if ( pHalData->ReceiveConfig & RCR_APPFCS)
			recvframe_pull_tail(precvframe_if2, IEEE80211_FCS_LEN);

		if (pattrib->physt)
			update_recvframe_phyinfo(precvframe_if2, pphy_status);

		if(rtw_recv_entry(precvframe_if2) != _SUCCESS)
		{
			RT_TRACE(_module_rtl871x_recv_c_,_drv_err_,
				("recvbuf2recvframe: rtw_recv_entry(precvframe) != _SUCCESS\n"));
		}
	}

	if (precvframe->u.hdr.attrib.physt)
		update_recvframe_phyinfo(precvframe, pphy_status);

	ret = rtw_recv_entry(precvframe);
#endif

	return ret;

}


static void rtl8723bs_recv_tasklet(void *priv)
{
	PADAPTER			padapter;
	PHAL_DATA_TYPE		pHalData;
	struct recv_priv		*precvpriv;
	struct recv_buf		*precvbuf;
	union recv_frame		*precvframe;
	struct recv_frame_hdr	*phdr;
	struct rx_pkt_attrib	*pattrib;
	_irqL	irql;
	u8		*ptr;
	u32		pkt_len, pkt_offset, skb_len, alloc_sz;
	_pkt		*pkt_copy = NULL;
	u8		shift_sz = 0, rx_report_sz = 0;


	padapter = (PADAPTER)priv;
	pHalData = GET_HAL_DATA(padapter);
	precvpriv = &padapter->recvpriv;

	do {
		precvbuf = rtw_dequeue_recvbuf(&precvpriv->recv_buf_pending_queue);
		if (NULL == precvbuf) break;

		ptr = precvbuf->pdata;

		while (ptr < precvbuf->ptail)
		{
			precvframe = rtw_alloc_recvframe(&precvpriv->free_recv_queue);
			if (precvframe == NULL) {
				RT_TRACE(_module_rtl871x_recv_c_, _drv_err_, ("%s: no enough recv frame!\n",__FUNCTION__));
				rtw_enqueue_recvbuf_to_head(precvbuf, &precvpriv->recv_buf_pending_queue);

				// The case of can't allocte recvframe should be temporary,
				// schedule again and hope recvframe is available next time.
#ifdef PLATFORM_LINUX
				tasklet_schedule(&precvpriv->recv_tasklet);
#endif
				return;
			}

			//rx desc parsing
			update_recvframe_attrib_8723b(padapter, precvframe, (struct recv_stat*)ptr);

			pattrib = &precvframe->u.hdr.attrib;

			// fix Hardware RX data error, drop whole recv_buffer
			if ((!(pHalData->ReceiveConfig & RCR_ACRC32)) && pattrib->crc_err)
			{
				#if !(MP_DRIVER==1)
				DBG_8192C("%s()-%d: RX Warning! rx CRC ERROR !!\n", __FUNCTION__, __LINE__);
				#endif
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				break;
			}

			if (pHalData->ReceiveConfig & RCR_APP_BA_SSN)
				rx_report_sz = RXDESC_SIZE + 4 + pattrib->drvinfo_sz;
			else
				rx_report_sz = RXDESC_SIZE + pattrib->drvinfo_sz;

			pkt_offset = rx_report_sz + pattrib->pkt_len;

			if ((ptr + pkt_offset) > precvbuf->ptail) {
				DBG_8192C("%s()-%d: : next pkt len(%p,%d) exceed ptail(%p)!\n", __FUNCTION__, __LINE__, ptr, pkt_offset, precvbuf->ptail);
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				break;
			}

			if ((pattrib->crc_err) || (pattrib->icv_err))
			{
#ifdef CONFIG_MP_INCLUDED
				if (padapter->registrypriv.mp_mode == 1)
				{
					if ((check_fwstate(&padapter->mlmepriv, WIFI_MP_STATE) == _TRUE))//&&(padapter->mppriv.check_mp_pkt == 0))
					{
						if (pattrib->crc_err == 1)
							padapter->mppriv.rx_crcerrpktcount++;
					}
				}
#endif
				DBG_8192C("%s: crc_err=%d icv_err=%d, skip!\n", __FUNCTION__, pattrib->crc_err, pattrib->icv_err);
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
			}
			else
			{
				//	Modified by Albert 20101213
				//	For 8 bytes IP header alignment.
				if (pattrib->qos)	//	Qos data, wireless lan header length is 26
				{
					shift_sz = 6;
				}
				else
				{
					shift_sz = 0;
				}

				skb_len = pattrib->pkt_len;

				// for first fragment packet, driver need allocate 1536+drvinfo_sz+RXDESC_SIZE to defrag packet.
				// modify alloc_sz for recvive crc error packet by thomas 2011-06-02
				if((pattrib->mfrag == 1)&&(pattrib->frag_num == 0)){
					//alloc_sz = 1664;	//1664 is 128 alignment.
					if(skb_len <= 1650)
						alloc_sz = 1664;
					else
						alloc_sz = skb_len + 14;
				}
				else {
					alloc_sz = skb_len;
					//	6 is for IP header 8 bytes alignment in QoS packet case.
					//	8 is for skb->data 4 bytes alignment.
					alloc_sz += 14;
				}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)) // http://www.mail-archive.com/netdev@vger.kernel.org/msg17214.html
				pkt_copy = dev_alloc_skb(alloc_sz);
#else
				pkt_copy = netdev_alloc_skb(padapter->pnetdev, alloc_sz);
#endif
				if(pkt_copy)
				{
					pkt_copy->dev = padapter->pnetdev;
					precvframe->u.hdr.pkt = pkt_copy;
					skb_reserve( pkt_copy, 8 - ((SIZE_PTR)( pkt_copy->data ) & 7 ));//force pkt_copy->data at 8-byte alignment address
					skb_reserve( pkt_copy, shift_sz );//force ip_hdr at 8-byte alignment address according to shift_sz.
					_rtw_memcpy(pkt_copy->data, (ptr + rx_report_sz), skb_len);
					precvframe->u.hdr.rx_head = pkt_copy->head;
					precvframe->u.hdr.rx_data = precvframe->u.hdr.rx_tail = pkt_copy->data;
					precvframe->u.hdr.rx_end = skb_end_pointer(pkt_copy);
				}
				else
				{
					if((pattrib->mfrag == 1)&&(pattrib->frag_num == 0))
					{
						DBG_8192C("rtl8723bs_recv_tasklet: alloc_skb fail , drop frag frame \n");
						rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
						break;
					}

					precvframe->u.hdr.pkt = skb_clone(precvbuf->pskb, GFP_ATOMIC);
					if(precvframe->u.hdr.pkt)
					{
						_pkt	*pkt_clone = precvframe->u.hdr.pkt;

						pkt_clone->data = ptr + rx_report_sz;
						skb_reset_tail_pointer(pkt_clone);
						precvframe->u.hdr.rx_head = precvframe->u.hdr.rx_data = precvframe->u.hdr.rx_tail
							= pkt_clone->data;
						precvframe->u.hdr.rx_end =  pkt_clone->data + skb_len;
					}
					else
					{
						DBG_8192C("rtl8723bs_recv_tasklet: skb_clone fail\n");
						rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
						break;
					}
				}

				recvframe_put(precvframe, skb_len);
				//recvframe_pull(precvframe, drvinfo_sz + RXDESC_SIZE);

				if (pHalData->ReceiveConfig & RCR_APPFCS)
					recvframe_pull_tail(precvframe, IEEE80211_FCS_LEN);

				// move to drv info position
				ptr += RXDESC_SIZE;

				// update drv info
				if (pHalData->ReceiveConfig & RCR_APP_BA_SSN) {
					//rtl8723s_update_bassn(padapter, pdrvinfo);
					ptr += 4;
				}

#ifdef CONFIG_C2H_PACKET_EN
				if(pattrib->pkt_rpt_type == NORMAL_RX)//Normal rx packet
				{
#endif
#ifdef CONFIG_CONCURRENT_MODE
					if(rtw_buddy_adapter_up(padapter))
					{
						if(pre_recv_entry(precvframe, precvbuf, (struct phy_stat*)ptr) != _SUCCESS)
						{
							RT_TRACE(_module_rtl871x_recv_c_,_drv_err_,
								("recvbuf2recvframe: recv_entry(precvframe) != _SUCCESS\n"));
						}
					}
					else
#endif
					{
						if (pattrib->physt)
							update_recvframe_phyinfo(precvframe, (struct phy_stat*)ptr);

						if (rtw_recv_entry(precvframe) != _SUCCESS)
						{
							RT_TRACE(_module_rtl871x_recv_c_, _drv_info_, ("%s: rtw_recv_entry(precvframe) != _SUCCESS\n",__FUNCTION__));
						}
					}
#ifdef CONFIG_C2H_PACKET_EN
				}
				else if(pattrib->pkt_rpt_type == C2H_PACKET)
				{
					C2H_EVT_HDR 	C2hEvent;

					u16 len_c2h = pattrib->pkt_len;
					u8 *pbuf_c2h = precvframe->u.hdr.rx_data;
					u8 *pdata_c2h;

					C2hEvent.CmdID = pbuf_c2h[0];
					C2hEvent.CmdSeq = pbuf_c2h[1];
					C2hEvent.CmdLen = (len_c2h -2);
					pdata_c2h = pbuf_c2h+2;

					if(C2hEvent.CmdID == C2H_CCX_TX_RPT)
					{
						CCX_FwC2HTxRpt_8723b(padapter, pdata_c2h, C2hEvent.CmdLen);
					}
					else
					{
						rtl8723bs_c2h_packet_handler(padapter, precvframe->u.hdr.rx_data, pattrib->pkt_len);
					}
					rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				}
#endif
			}

			// Page size of receive package is 128 bytes alignment =>DMA AGG
			// refer to _InitTransferPageSize()
			pkt_offset = _RND8(pkt_offset);
			precvbuf->pdata += pkt_offset;
			ptr = precvbuf->pdata;
			precvframe = NULL;
			pkt_copy = NULL;
		}

		rtw_enqueue_recvbuf(precvbuf, &precvpriv->free_recv_buf_queue);
	} while (1);

}
#else
static s32 pre_recv_entry(union recv_frame *precvframe, struct recv_buf *precvbuf, struct phy_stat *pphy_status)
{
	s32 ret=_SUCCESS;
#ifdef CONFIG_CONCURRENT_MODE
	u8 *primary_myid, *secondary_myid, *paddr1;
	union recv_frame	*precvframe_if2 = NULL;
	_adapter *primary_padapter = precvframe->u.hdr.adapter;
	_adapter *secondary_padapter = primary_padapter->pbuddy_adapter;
	struct recv_priv *precvpriv = &primary_padapter->recvpriv;
	_queue *pfree_recv_queue = &precvpriv->free_recv_queue;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(primary_padapter);

	if(!secondary_padapter)
		return ret;

	paddr1 = GetAddr1Ptr(precvframe->u.hdr.rx_data);

	if(IS_MCAST(paddr1) == _FALSE)//unicast packets
	{
		//primary_myid = myid(&primary_padapter->eeprompriv);
		secondary_myid = myid(&secondary_padapter->eeprompriv);

		if(_rtw_memcmp(paddr1, secondary_myid, ETH_ALEN))
		{
			//change to secondary interface
			precvframe->u.hdr.adapter = secondary_padapter;
		}

		//ret = recv_entry(precvframe);

	}
	else // Handle BC/MC Packets
	{
		//clone/copy to if2
		_pkt	 *pkt_copy = NULL;
		struct rx_pkt_attrib *pattrib = NULL;

		precvframe_if2 = rtw_alloc_recvframe(pfree_recv_queue);

		if(!precvframe_if2)
			return _FAIL;

		precvframe_if2->u.hdr.adapter = secondary_padapter;
		_rtw_memcpy(&precvframe_if2->u.hdr.attrib, &precvframe->u.hdr.attrib, sizeof(struct rx_pkt_attrib));
		pattrib = &precvframe_if2->u.hdr.attrib;

		//driver need to set skb len for skb_copy().
		//If skb->len is zero, skb_copy() will not copy data from original skb.
		skb_put(precvframe->u.hdr.pkt, pattrib->pkt_len);

		pkt_copy = skb_copy( precvframe->u.hdr.pkt, GFP_ATOMIC);
		if (pkt_copy == NULL)
		{
			if((pattrib->mfrag == 1)&&(pattrib->frag_num == 0))
			{
				DBG_8192C("pre_recv_entry(): skb_copy fail , drop frag frame \n");
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				return ret;
			}

			pkt_copy = skb_clone( precvframe->u.hdr.pkt, GFP_ATOMIC);
			if(pkt_copy == NULL)
			{
				DBG_8192C("pre_recv_entry(): skb_clone fail , drop frame\n");
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				return ret;
			}
		}

		pkt_copy->dev = secondary_padapter->pnetdev;

		precvframe_if2->u.hdr.pkt = pkt_copy;
		precvframe_if2->u.hdr.rx_head = pkt_copy->head;
		precvframe_if2->u.hdr.rx_data = pkt_copy->data;
		precvframe_if2->u.hdr.rx_tail = skb_tail_pointer(pkt_copy);
		precvframe_if2->u.hdr.rx_end = skb_end_pointer(pkt_copy);
		precvframe_if2->u.hdr.len = pkt_copy->len;

		//recvframe_put(precvframe_if2, pattrib->pkt_len);

		if ( pHalData->ReceiveConfig & RCR_APPFCS)
			recvframe_pull_tail(precvframe_if2, IEEE80211_FCS_LEN);

		if (pattrib->physt)
			update_recvframe_phyinfo(precvframe_if2, pphy_status);

		if(rtw_recv_entry(precvframe_if2) != _SUCCESS)
		{
			RT_TRACE(_module_rtl871x_recv_c_,_drv_err_,
				("recvbuf2recvframe: rtw_recv_entry(precvframe) != _SUCCESS\n"));
		}
	}

	if (precvframe->u.hdr.attrib.physt)
		update_recvframe_phyinfo(precvframe, pphy_status);

	ret = rtw_recv_entry(precvframe);
#endif

	return ret;

}

static void rtl8723bs_recv_tasklet(void *priv)
{
	PADAPTER				padapter;
	PHAL_DATA_TYPE			pHalData;
	struct recv_priv		*precvpriv;
	struct recv_buf			*precvbuf;
	union recv_frame		*precvframe;
	struct recv_frame_hdr	*phdr;
	struct rx_pkt_attrib	*pattrib;
	u8			*ptr;
	_pkt		*ppkt;
	u32			pkt_offset;
	_irqL		irql;


	padapter = (PADAPTER)priv;
	pHalData = GET_HAL_DATA(padapter);
	precvpriv = &padapter->recvpriv;

	do {
		precvbuf = rtw_dequeue_recvbuf(&precvpriv->recv_buf_pending_queue);
		if (NULL == precvbuf) break;

		ptr = precvbuf->pdata;

		while (ptr < precvbuf->ptail)
		{
			precvframe = rtw_alloc_recvframe(&precvpriv->free_recv_queue);
			if (precvframe == NULL) {
				RT_TRACE(_module_rtl871x_recv_c_, _drv_err_, ("rtl8723bs_recv_tasklet: no enough recv frame!\n"));
				rtw_enqueue_recvbuf_to_head(precvbuf, &precvpriv->recv_buf_pending_queue);

				// The case of can't allocte recvframe should be temporary,
				// schedule again and hope recvframe is available next time.
#ifdef PLATFORM_LINUX
				tasklet_schedule(&precvpriv->recv_tasklet);
#endif
				return;
			}

			phdr = &precvframe->u.hdr;
			pattrib = &phdr->attrib;

			update_recvframe_attrib_8723b(padapter, precvframe, (struct recv_stat*)ptr);

#if 0
			{
				int i, len = 64;
				u8 *pptr = ptr;

				if((*(pptr + RXDESC_SIZE + pattrib->drvinfo_sz) != 0x80) && (*(pptr + RXDESC_SIZE + pattrib->drvinfo_sz) != 0x40))
				{
					DBG_871X("##############RxDESC############### \n");
					for(i=0; i<32;i=i+16)
						DBG_871X("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:\n", *(pptr+i),
						*(pptr+i+1), *(pptr+i+2) ,*(pptr+i+3) ,*(pptr+i+4),*(pptr+i+5), *(pptr+i+6), *(pptr+i+7), *(pptr+i+8), *(pptr+i+9), *(pptr+i+10),
						 *(pptr+i+11), *(pptr+i+12), *(pptr+i+13), *(pptr+i+14), *(pptr+i+15));

					if(pattrib->pkt_len < 100)
						len = pattrib->pkt_len;
					pptr = ptr + RXDESC_SIZE + pattrib->drvinfo_sz;
					DBG_871X("##############Len=%d############### \n", pattrib->pkt_len);
					for(i=0; i<len;i=i+16)
						DBG_871X("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:\n", *(pptr+i),
						*(pptr+i+1), *(pptr+i+2) ,*(pptr+i+3) ,*(pptr+i+4),*(pptr+i+5), *(pptr+i+6), *(pptr+i+7), *(pptr+i+8), *(pptr+i+9), *(pptr+i+10),
						 *(pptr+i+11), *(pptr+i+12), *(pptr+i+13), *(pptr+i+14), *(pptr+i+15));
					DBG_871X("############################# \n");
				}
			}
#endif

			// fix Hardware RX data error, drop whole recv_buffer
			if ((!(pHalData->ReceiveConfig & RCR_ACRC32)) && pattrib->crc_err)
			{
				DBG_8192C("%s()-%d: RX Warning! rx CRC ERROR !!\n", __FUNCTION__, __LINE__);
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				break;
			}

			pkt_offset = RXDESC_SIZE + pattrib->drvinfo_sz + pattrib->pkt_len;
#if 0 // reduce check to speed up
			if ((ptr + pkt_offset) > precvbuf->ptail) {
				RT_TRACE(_module_rtl871x_recv_c_, _drv_err_,
						("%s: next pkt len(%p,%d) exceed ptail(%p)!\n",
						__FUNCTION__, ptr, pkt_offset, precvbuf->ptail));
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				break;
			}
#endif

			if ((pattrib->crc_err) || (pattrib->icv_err))
			{
#ifdef CONFIG_MP_INCLUDED
				if (padapter->registrypriv.mp_mode == 1)
				{
					if ((check_fwstate(&padapter->mlmepriv, WIFI_MP_STATE) == _TRUE))//&&(padapter->mppriv.check_mp_pkt == 0))
					{
						if (pattrib->crc_err == 1)
							padapter->mppriv.rx_crcerrpktcount++;
					}
				}
#endif
				DBG_8192C("%s: crc_err=%d icv_err=%d, skip!\n", __FUNCTION__, pattrib->crc_err, pattrib->icv_err);
				rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
			}
			else
			{
				ppkt = skb_clone(precvbuf->pskb, GFP_ATOMIC);
				if (ppkt == NULL)
				{
					RT_TRACE(_module_rtl871x_recv_c_, _drv_crit_, ("rtl8723bs_recv_tasklet: no enough memory to allocate SKB!\n"));
					rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
					rtw_enqueue_recvbuf_to_head(precvbuf, &precvpriv->recv_buf_pending_queue);

					// The case of can't allocte skb is serious and may never be recovered,
					// once bDriverStopped is enable, this task should be stopped.
					if (padapter->bDriverStopped == _FALSE) {
#ifdef PLATFORM_LINUX
						tasklet_schedule(&precvpriv->recv_tasklet);
#endif
					}

					return;
				}

				phdr->pkt = ppkt;
				phdr->len = 0;
				phdr->rx_head = precvbuf->phead;
				phdr->rx_data = phdr->rx_tail = precvbuf->pdata;
				phdr->rx_end = precvbuf->pend;
				recvframe_put(precvframe, pkt_offset);
				recvframe_pull(precvframe, RXDESC_SIZE + pattrib->drvinfo_sz);
				if (pHalData->ReceiveConfig & RCR_APPFCS)
					recvframe_pull_tail(precvframe, IEEE80211_FCS_LEN);

				// move to drv info position
				ptr += RXDESC_SIZE;

				// update drv info
				if (pHalData->ReceiveConfig & RCR_APP_BA_SSN) {
//					rtl8723s_update_bassn(padapter, pdrvinfo);
					ptr += 4;
				}

				if(pattrib->pkt_rpt_type == NORMAL_RX)//Normal rx packet
				{
#ifdef CONFIG_CONCURRENT_MODE
					if(rtw_buddy_adapter_up(padapter))
					{
						if(pre_recv_entry(precvframe, precvbuf, (struct phy_stat*)ptr) != _SUCCESS)
						{
							RT_TRACE(_module_rtl871x_recv_c_,_drv_err_,
								("recvbuf2recvframe: recv_entry(precvframe) != _SUCCESS\n"));
						}
					}
					else
#endif
					{
						if (pattrib->physt)
							update_recvframe_phyinfo(precvframe, (struct phy_stat*)ptr);

						if (rtw_recv_entry(precvframe) != _SUCCESS)
						{
							RT_TRACE(_module_rtl871x_recv_c_, _drv_info_, ("rtl8723bs_recv_tasklet: rtw_recv_entry(precvframe) != _SUCCESS\n"));
						}
					}
				}
#ifdef CONFIG_C2H_PACKET_EN
				else if(pattrib->pkt_rpt_type == C2H_PACKET)
				{
					rtl8723bs_c2h_packet_handler(padapter, precvframe->u.hdr.rx_data, pattrib->pkt_len);
					rtw_free_recvframe(precvframe, &precvpriv->free_recv_queue);
				}
#endif
			}

			// Page size of receive package is 128 bytes alignment => DMA agg
			// refer to _InitTransferPageSize()
			//pkt_offset = _RND128(pkt_offset);
			pkt_offset = _RND8(pkt_offset);
			precvbuf->pdata += pkt_offset;
			ptr = precvbuf->pdata;
		}

		dev_kfree_skb_any(precvbuf->pskb);
		precvbuf->pskb = NULL;
		rtw_enqueue_recvbuf(precvbuf, &precvpriv->free_recv_buf_queue);
	} while (1);
}
#endif

/*
 * Initialize recv private variable for hardware dependent
 * 1. recv buf
 * 2. recv tasklet
 *
 */
s32 rtl8723bs_init_recv_priv(PADAPTER padapter)
{
	s32			res= _SUCCESS;
	struct recv_priv	*precvpriv = &padapter->recvpriv;

	//3 init tasklet
#ifdef PLATFORM_LINUX
	tasklet_init(&precvpriv->recv_tasklet,
	     (void(*)(unsigned long))rtl8723bs_recv_tasklet,
	     (unsigned long)padapter);
#endif

	return res;
}

/*
 * Free recv private variable of hardware dependent
 * 1. recv buf
 * 2. recv tasklet
 *
 */
void rtl8723bs_free_recv_priv(PADAPTER padapter)
{
	struct recv_priv	*precvpriv = &padapter->recvpriv;

	//3 kill tasklet
#ifdef PLATFORM_LINUX
	tasklet_kill(&precvpriv->recv_tasklet);
#endif
}

