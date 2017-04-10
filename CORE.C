/*****************************************************************************/
/*       FileName  :   CORE.C                                                */
/*       Content   :   DSA-208 CORE Module                                   */
/*       Date      :   Fri  02-22-2002                                       */
/*                     DSASoftWare(c)                                        */
/*                     CopyRight 2002             DSA-GROUP                  */
/*****************************************************************************/









/************************************************/
/* Core_ObjectNo_To_UnitAddr      function      */
/************************************************/
/*===========================================================================*/
BYTE Ex_YK_CDTBB_ObjectNo_To_UnitAddr(void)
{
  unsigned short temp_rnd,temp_rnd1;

  for(temp_rnd=0;temp_rnd<14;temp_rnd++)
  {
      if(  (exchange_buf[5] >= Ex_YK_start[temp_rnd])
         &&(exchange_buf[5] <= Ex_YK_end[temp_rnd]) )
      {
          for(temp_rnd1  = byte0(port_info[temp_rnd].mirror_unit);
              temp_rnd1 <= byte1(port_info[temp_rnd].mirror_unit);
              temp_rnd1++ )
          {
              if(   (exchange_buf[5] >= unit_info[temp_rnd1].yk_start)
                  &&(exchange_buf[5] < (unit_info[temp_rnd1].yk_start+unit_info[temp_rnd1].yk_num))
                )
              {
                  exchange_buf[17] = exchange_buf[5] - unit_info[temp_rnd1].yk_start;     // yk_no in unit
                  exchange_buf[5]  = temp_rnd1-byte0(port_info[temp_rnd].mirror_unit);    // unit addr
                  exchange_target_port=temp_rnd;
                  return YES;
              }
          }
      }
  }
  
  return NO;
}










/************************************************/
/* Ex_Produce_Transmit_Info     function        */
/* Input :    exchange_buf[xxx]                 */
/* Output:    port_transmit[port][xxx]          */
/************************************************/
/*===========================================================================*/
BYTE Ex_Produce_Transmit_Info(void)      ;;;����_����_ת��_��Ϣ
{
    unsigned short temp_rnd;
    unsigned short temp_rnd1;
    unsigned char *temp_pt;

    if(exchange_target_port<14)             ;;;������Ŀ�Ŀ�ֻ��14��
    {
        if(port_transmit_flag[exchange_target_port] == 0x55)
        {
            temp_pt = &port_transmit[exchange_target_port][0];         ;;;������汣����Ҫ��¼����ֵ
            for(temp_rnd=1;temp_rnd<16;temp_rnd++)
            {
                temp_pt[temp_rnd]=exchange_buf[temp_rnd];
            }

            byte0(temp_rnd1)=exchange_buf[9];
            byte1(temp_rnd1)=exchange_buf[10];
            temp_rnd1=temp_rnd1+16;
            for(temp_rnd=16;temp_rnd<temp_rnd1;temp_rnd++)
            {
                temp_pt[temp_rnd]=exchange_buf[temp_rnd];          ;;;��һ����������
            }
            port_transmit_flag[exchange_target_port] = 0xaa;   ;;;�����������ݾ͸������
            return YES;
        }   
        else
            return NO;
    }
    else
    {
        if(set_transmit_flag == 0x55)
        {
            temp_pt = &set_transmit[0]; 
            for(temp_rnd=1;temp_rnd<16;temp_rnd++)
            {
                temp_pt[temp_rnd]=exchange_buf[temp_rnd];
            }

            byte0(temp_rnd1)=exchange_buf[9];
            byte1(temp_rnd1)=exchange_buf[10];
            if(temp_rnd1>250) 
                return NO;
            temp_rnd1=temp_rnd1+16;
            for(temp_rnd=16;temp_rnd<temp_rnd1;temp_rnd++)
            {
                temp_pt[temp_rnd]=exchange_buf[temp_rnd];
            }
            set_transmit_flag = 0xaa;
            return YES;
        }   
        else
            return NO;
    }
}











void Ex_scan_yk_range_for_port()
{
  unsigned short temp_rnd,temp_rnd1;
  unsigned char  the_start_flag[14],the_end_flag[14];
  
  for(temp_rnd=0;temp_rnd<14;temp_rnd++)
  { 
     the_start_flag[temp_rnd] = 0;
     the_end_flag[temp_rnd]   = 0;
     
     Ex_YK_start[temp_rnd]   = 1;
     Ex_YK_end[temp_rnd]     = 0;
  }

  for(temp_rnd=0;temp_rnd<256;temp_rnd++)	//-�ܹ�256����Ԫ���Զ���,�����ǵ�һ�ζԶ���ĵ�Ԫ���д���
  {
      if(unit_info[temp_rnd].yk_num!=0)	//-û������Ϊ0˵����ң�ض�����
      {
          for(temp_rnd1=0;temp_rnd1<14;temp_rnd1++)
          {
              if(  (temp_rnd>=byte0(port_info[temp_rnd1].mirror_unit))		//-��ʼ��Ԫ��
                 &&(temp_rnd<=byte1(port_info[temp_rnd1].mirror_unit)) )	//-������Ԫ��,,�����ң�صĵ�Ԫ�Ƿ���ͨѶ�����÷�Χ��
              {		//-ÿһ����Ԫֻ���Ӧһ��ͨѶ��,�������������úõ�,�ض���ͨѶ�ڹ�ԼҲ����������
                  if(the_start_flag[temp_rnd1]==0)
                  {
                      the_start_flag[temp_rnd1]=0xff;	//-ff˵�����ͨѶ����ң������
                      Ex_YK_start[temp_rnd1]=unit_info[temp_rnd].yk_start;
                  }
                  
                  if(unit_info[temp_rnd].yk_start<Ex_YK_start[temp_rnd1])
                  {
                      Ex_YK_start[temp_rnd1]=unit_info[temp_rnd].yk_start;	//-��¼���ǽ������ͨѶ�������е�Ԫң�ؿ�ʼ��С��ֵ
                  }

                  if(the_end_flag[temp_rnd1]==0)
                  {
                      the_end_flag[temp_rnd1]=0xff;
                      Ex_YK_end[temp_rnd1] = unit_info[temp_rnd].yk_start + unit_info[temp_rnd].yk_num-1;
                  }
                  if((unit_info[temp_rnd].yk_start+unit_info[temp_rnd].yk_num)>Ex_YK_end[temp_rnd1]) 
                      Ex_YK_end[temp_rnd1] = unit_info[temp_rnd].yk_start + unit_info[temp_rnd].yk_num-1; 	//-��¼���Ǳ�ͨѶ�������е�Ԫң�غ������Ǹ�   
              }
          }
      }
  }
}

void core_init()
{
  yx_chg_in_pt = 0;
  yx_soe_in_pt = 0;
  for(temp_loop=0;temp_loop<4;temp_loop++)
  {
     yx_chg_tr_ex_pt[temp_loop] = 0;	//-���ںܶ������Ԫ��ʼ�ڼ䶼��0
     yx_soe_tr_ex_pt[temp_loop] = 0;
     yx_chg_tr_in_pt[temp_loop] = 0;
     yx_soe_tr_in_pt[temp_loop] = 0;
  }

  BH_Report_Bank_Sav_Ptr=0;
  
  for(temp_loop=0;temp_loop<12;temp_loop++)
  {
      BH_Report_Bank_Tak_Ptr[temp_loop]=0;
  } 
}

void Ex_init()
{
  unsigned char temp_rnd;

  core_init();
  Ex_scan_yk_range_for_port();

  for(temp_rnd=0;temp_rnd<14;temp_rnd++)
  {
     port_transmit_flag[temp_rnd] = 0x55;	//-�˿ڷ��ͱ�־��ʼ״̬Ϊ55,,�ǺǸ���ô����������־λ,�����Ե�
     port_transmit[temp_rnd][0]   = PORT_EXCHANGE_STA_IDLE;		//-���е�
  }  
}


//Ӧ�� core  ǰ�����ͷ� temp_loop,temp_loop1,temp_int,temp_lp_int 

void core_get_bh_bank_report()
{
	BYTE need_refind;
	
    if(Core_Src_Unit>=BH_REPORT_BANK_SIZE)  Core_Src_Unit=0;

    Core_Src_Pt_B=&BH_Saved_Report_Bank[Core_Src_Unit][0];
    need_refind=NO;
re_find:
    while((Core_Src_Pt_B[4]!=( (Core_Src_Pt_B[0]+Core_Src_Pt_B[1]+Core_Src_Pt_B[2]+Core_Src_Pt_B[3])&0xff ))
        ||(Core_Src_Pt_B[2]!=0)||(need_refind==YES))
    {
    	need_refind=NO;
        Core_Src_Unit++;
        if(Core_Src_Unit>=BH_REPORT_BANK_SIZE) Core_Src_Unit=0;
        if(Core_Src_Unit==BH_Report_Bank_Sav_Ptr)
        {
            Core_Src_Len=0;
            return;
        }
        Core_Src_Pt_B=&BH_Saved_Report_Bank[Core_Src_Unit][0];
    }
    
    byte1(Core_Temp_Loop)=Core_Src_Pt_B[6];
    if(byte1(Core_Temp_Loop)>10) 
    {
    	need_refind=YES;
    	goto re_find;
    }	
    for(byte0(Core_Temp_Loop)=0;byte0(Core_Temp_Loop)<byte1(Core_Temp_Loop);byte0(Core_Temp_Loop)++)
    {
    	if((Core_Src_Pt_B[2]!=byte0(Core_Temp_Loop))||(Core_Src_Pt_B[3]>26)
    	 ||(Core_Src_Pt_B[4]!=( (Core_Src_Pt_B[0]+Core_Src_Pt_B[1]+Core_Src_Pt_B[2]+Core_Src_Pt_B[3])&0xff )))  // frame_NO err
    	{
	    	need_refind=YES;
    		goto re_find;
    	}

		if(byte0(Core_Temp_Loop)==0)
		{
			if(Core_Src_Pt_B[3]>2)
				byte1(Core_Temp_ShortInt)=Core_Src_Pt_B[3]-2;
			else
				byte1(Core_Temp_ShortInt)=0;
			for(byte0(Core_Temp_ShortInt)=0;byte0(Core_Temp_ShortInt)<byte1(Core_Temp_ShortInt);byte0(Core_Temp_ShortInt)++)
			{
				BH_Bank_Report[byte0(Core_Temp_ShortInt)]=Core_Src_Pt_B[byte0(Core_Temp_ShortInt)+8];
			}
			Core_Src_Len=byte1(Core_Temp_ShortInt);
		    BH_Bank_Report[509]=Core_Src_Pt_B[1];
		    BH_Bank_Report[510]=Core_Src_Pt_B[0];
		    BH_Bank_Report[511]=Core_Src_Pt_B[5];
		}
		else
		{
			if((Core_Src_Pt_B[0]!=BH_Bank_Report[510])||(Core_Src_Pt_B[5]!=BH_Bank_Report[511])
			 ||(Core_Src_Pt_B[1]!=BH_Bank_Report[509]))
			{
		    	need_refind=YES;
    			goto re_find;
			}
			
			byte1(Core_Temp_ShortInt)=Core_Src_Pt_B[3];
			for(byte0(Core_Temp_ShortInt)=0;byte0(Core_Temp_ShortInt)<byte1(Core_Temp_ShortInt);byte0(Core_Temp_ShortInt)++)
			{
				BH_Bank_Report[Core_Src_Len+byte0(Core_Temp_ShortInt)]=Core_Src_Pt_B[byte0(Core_Temp_ShortInt)+6];
			}
			Core_Src_Len+=byte1(Core_Temp_ShortInt);
		}	

        Core_Src_Unit++;
        if(Core_Src_Unit>=BH_REPORT_BANK_SIZE) Core_Src_Unit=0;
        Core_Src_Pt_B=&BH_Saved_Report_Bank[Core_Src_Unit][0];
    }
    
}

void core_get_yx_set_unit()
{
  if((Core_Src_Unit>0x1f)&&(Core_Src_Unit<0x30))
     Core_Temp_Loop = 4;
  else
     Core_Temp_Loop = unit_info[Core_Src_Unit].yx_num;	//-�����Ǹ�����,��ͬ��Ԫ�����ʲ�ͬ,���õ�ֵҲ�в�ͬ
  //Core_Src_Unit Core_Src_Pt_B
  Core_Temp_ShortInt= unit_info[Core_Src_Unit].yx_start;
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<Core_Temp_Loop;Core_Temp_Loop1++)
  {
     *Core_Src_Pt_B =  byte0(YX_property[Core_Temp_ShortInt + Core_Temp_Loop1]);	//-��Щֵ������ǰ�ƶ��õ�,Ҫ��ʱ����
      Core_Src_Pt_B ++;
     *Core_Src_Pt_B = byte1(YX_property[Core_Temp_ShortInt + Core_Temp_Loop1]);
      Core_Src_Pt_B ++;    
  }
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<Core_Temp_Loop;Core_Temp_Loop1++)
  {
     *Core_Src_Pt_B =  byte0(YX_YM[Core_Temp_ShortInt+Core_Temp_Loop1]);
      Core_Src_Pt_B ++;
     *Core_Src_Pt_B = byte1(YX_YM[Core_Temp_ShortInt+Core_Temp_Loop1]);
      Core_Src_Pt_B ++;    
  }

  Core_Temp_ShortInt = Core_Temp_ShortInt*8;
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<(Core_Temp_Loop*8);Core_Temp_Loop1++)
  {
     if((Core_Temp_ShortInt + Core_Temp_Loop1)<256)
     {
        *Core_Src_Pt_B =  byte0(YX_double[Core_Temp_ShortInt + Core_Temp_Loop1]);
         Core_Src_Pt_B ++;
        *Core_Src_Pt_B = byte1(YX_double[Core_Temp_ShortInt + Core_Temp_Loop1]);
         Core_Src_Pt_B ++;    
     }
     else
     {
        *Core_Src_Pt_B =  0xc8;
         Core_Src_Pt_B ++;
        *Core_Src_Pt_B =  0xc8;
         Core_Src_Pt_B ++;    
     }
  }

  Core_Temp_ShortInt = Core_Temp_ShortInt*2;
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<(Core_Temp_Loop*16);Core_Temp_Loop1++)
  {
     if((Core_Temp_ShortInt + Core_Temp_Loop1)<512)
     {
        *Core_Src_Pt_B =  byte0(YX_delay[Core_Temp_ShortInt + Core_Temp_Loop1]);
         Core_Src_Pt_B ++;
        *Core_Src_Pt_B = byte1(YX_delay[Core_Temp_ShortInt + Core_Temp_Loop1]);    
         Core_Src_Pt_B ++;
     }
     else
     {
        *Core_Src_Pt_B =  0xc8;
         Core_Src_Pt_B ++;
        *Core_Src_Pt_B =  0x00;    
         Core_Src_Pt_B ++;
     }
  }
}

void core_get_yc_set_unit()
{
  if((Core_Src_Unit>0x0f)&&(Core_Src_Unit<0x20))
      Core_Temp_Loop = 6;
  else
      Core_Temp_Loop = unit_info[Core_Src_Unit].yc_line_num; //-�еķ�Χ�ڳ�����������,�е��ǿ����޸ĵ�

  //Core_Src_Unit Core_Src_Pt_B
  Core_Temp_ShortInt = unit_info[Core_Src_Unit].yc_line_start;  //-�����ֵ����һ����Ԫ������ֵ
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<Core_Temp_Loop;Core_Temp_Loop1++)
  {
     if(Core_Temp_Loop1<unit_info[Core_Src_Unit].yc_line_num)  //-����Ϊ��λ,,˫�����Ƹ���,ȡС��
     {
       *Core_Src_Pt_B =  YC_coef_table[(Core_Temp_ShortInt + Core_Temp_Loop1)*4];
        Core_Src_Pt_B ++;
       *Core_Src_Pt_B =  YC_coef_table[(Core_Temp_ShortInt + Core_Temp_Loop1)*4 + 1];
        Core_Src_Pt_B ++;    
     }
     else
     {
       *Core_Src_Pt_B =  0;
        Core_Src_Pt_B ++;
       *Core_Src_Pt_B =  0;
        Core_Src_Pt_B ++;    
     }
  }
}

void core_update_YC()
{
  Core_Temp_Loop = unit_info[Core_Src_Unit].yc_val_num;  //-������������õĳ���
  Core_Det_Pt = (unsigned short *)&(YC_State[unit_info[Core_Src_Unit].yc_val_start]); //-�ռ䲻��Ҫ,����һ����ֵ����,��Ҫ����ƫ����,������ʱʱ���ڵ�
  if(Core_Temp_Loop>Core_Src_Len)                                                     //-�ؼ���ʱЧ�� 
     Core_Temp_Loop = Core_Src_Len; //-��������¼�������������,�������,����,,���˺���ľ�����
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<Core_Temp_Loop;Core_Temp_Loop1++)
  {
     *Core_Det_Pt   =  (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;   //-��һ���ֿ����͵�����ƴ�ӻָ�����
     Core_Src_Pt_B += 2;
     Core_Det_Pt    ++;
  }
    
}

void core_update_DC()
{
  Core_Temp_Loop = unit_info[Core_Src_Unit].dc_num;
  Core_Det_Pt = (unsigned short *)&(YC_State[unit_info[Core_Src_Unit].dc_start]);  //-����ң��״̬��ֵ
  if(Core_Temp_Loop>Core_Src_Len)
     Core_Temp_Loop = Core_Src_Len;  //-��ʶ����ô��Ķ���,���������Ϊ�˿ɿ�,�м��ֳ�ʲô���ܶ���
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<Core_Temp_Loop;Core_Temp_Loop1++)
  {
     *Core_Det_Pt   =  (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
     Core_Src_Pt_B += 2;
     Core_Det_Pt    ++;
  }
    
}
















/************************************************/
/* Input :  Core_Src_Len                        */  ;;;ʵ�ʱ仯��YX�м���B
/*          Core_Src_Pt_B                       */  ;;;port_report[]���׵�ַ,�����¼��YXֵ
/*          Core_Src_Unit                       */  ;;;��Ԫ��ַ
/* Output:                                      */
/* REG varible used: Core_Det_Pt                */
/*                   Core_Src_Pt                */
/*                   Core_Temp_Loop             */
/*                   Core_Temp_Loop1            */
/*                   Core_Temp_ShortInt         */
/************************************************/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void core_update_YX()         //-��֤�仯�˵Ķ��޸�һ�����,������֮ǰ����û�б仯��Ҳ��дһ��,֮��ľ�û�б�Ҫ��
{
  unsigned short chg_buf0,chg_buf1;
  unsigned short  yx_buf0;

  Core_Det_Pt = (unsigned short *)&(YX_State[unit_info[Core_Src_Unit].yx_start]);   //-��̨װ������ң�ſ�ʼ�ĵ�ַ(ÿ��IED����һ��)

  Core_Temp_Loop   = unit_info[Core_Src_Unit].yx_num;     //-��̨װ�õ�ң����(��������)���õ�
  Core_Temp_Loop   = Core_Temp_Loop * 2;              	  //-�ֱ�����ֽ�
  if(Core_Temp_Loop>Core_Src_Len)       //-���������Ǹ�������,û�б仯�����ݾ�û�б�Ҫ��дһ����,���ǵ�,������ȡ��С
      Core_Temp_Loop = Core_Src_Len;       //-��Ȼ���õ�ң�ų��ȿ��ܺܳ����������Խ��յ�����󳤶�Ϊ���ݵ�,�Ҹ�ÿ̨װ�����ַʱ����Ҫ����,ֻҪ����������256����
  Core_Temp_Loop &= 0xfe;  //avoid odd   ���ⵥ�� ��Ȼ�������С�˾Ͱ�С����,����Ķ�ʧ,,�����ڲ���ֵ���Ǳ仯��YX��,����������������
  
  if(HOST_ZF_enable!=YES)       //-�� ����_�ٷ�_ʹ�� ����˼�� ;�ر�������֪ʶ�Ǳ����е�,�������������,?ֻ�п����Է����ܸ�����,������Ϊ��Ч��Ϣ
  {
     Core_Temp_Loop = Core_Temp_Loop/2;
     for(Core_Temp_Loop1=0;Core_Temp_Loop1<Core_Temp_Loop;Core_Temp_Loop1++)     //-����ط��ɶ��Ծͺ�ǿ,�Ҳ�Ҫ���࿼��Ӳ��
     {
        *Core_Det_Pt = (*(Core_Src_Pt_B+1))*256 + (*Core_Src_Pt_B);        //-�������ֽڵ�������ϳ���һ���ֵ����ݽ��б���,��YX�ź�����
         Core_Det_Pt ++;
         Core_Src_Pt_B +=2;         //-�ѱ���port_report[]�е����ݴ���YX_State[]��;����ط��͸�����������ֵ,�տ�ʼӦ���������,����һ��ʱ�������
     }    
     return;
  }
  
  Core_Src_Len = 0;             //-����Ϊ�Ĵ�����ʹ��
  while(Core_Temp_Loop>0)       //-ʵ����Ҫ���µĳ��̶�B;�м�����ط���ѭ�����,ÿ��ֻ����һ�����
  {
      Core_Src_Pt = &yx_change[yx_chg_in_pt].offset_no;           ;;;ȡ�����������ĵ�ַ
     *Core_Src_Pt =  unit_info[Core_Src_Unit].yx_start + Core_Src_Len;     ;;;��¼�ĸ�ң�ŷ����˱仯

      Core_Temp_ShortInt = 0x5555;

      chg_buf0  = 0;
      chg_buf1  = 0;        ;;;�ѵ���������Բ²⵽һ̨��������32·YX

      yx_buf0   = (*(Core_Src_Pt_B+1))*256 + (*Core_Src_Pt_B);      ;;;ȡ��һ��YX����Чֵ
      if(yx_buf0 != *Core_Det_Pt)         ;;;�Ƚ��Ƿ������и���,�Ⱦ���û�и���,�ս��յ���YX��������һ�ε����ݽ��бȽ�,��û�����ݸ��¾�����ȥ
      {
          Core_Temp_ShortInt = 0xaaaa;        ;;;˵����YX����
          chg_buf0           = yx_buf0 ^ (*Core_Det_Pt);   ;;;��¼�±仯YX����ֵ,���������1���Ƿ���0����û��,�������������1��ʾ�б仯,0��ʾû�б仯
      }
     *Core_Det_Pt     = yx_buf0;    ;;;��û�и��¾Ͱ���ֵ��дһ��,���ֿ���������Ĳ���ʵ���Ͼ�����һֱҪ�ҵ�,��һ����YX_State[]��д���ݵĵط�
      Core_Src_Pt_B   += 2;        ;;;����port_report[]����һ������
      Core_Det_Pt     ++;
      Core_Temp_Loop  -= 2;        ;;;�Ƚ���һ���ͼ�ȥһ��
      Core_Src_Len    ++;

      if((*Core_Src_Pt & 1) != 0)    ;;;��chg_bit0��Ӧ��λֵΪ1�����ʾ��λYX������λ
      {
          if(Core_Temp_ShortInt == 0xaaaa)
          {
             *Core_Src_Pt = *Core_Src_Pt / 2;   Core_Src_Pt++;
             *Core_Src_Pt = *(Core_Det_Pt-2);   Core_Src_Pt++;
             *Core_Src_Pt = *(Core_Det_Pt-1);   Core_Src_Pt++;
             *Core_Src_Pt = 0;                  Core_Src_Pt++; 
             *Core_Src_Pt = chg_buf0;
              yx_chg_in_pt ++;
              yx_chg_in_pt &= 0xff;         
              Core_Temp_ShortInt = 0x5555;
          }
      }
      else
      {
          if(Core_Temp_Loop>0)
          {
               yx_buf0 = ((*(Core_Src_Pt_B+1)))*256 + (*Core_Src_Pt_B);       ;;;�����λ����ң�ž�ȡֵ
               if(yx_buf0 != (*Core_Det_Pt))         ;;;��������û�б仯�Ⱦ���û�б仯
               {
                   Core_Temp_ShortInt = 0xaaaa;
                   chg_buf1           = yx_buf0 ^ (*Core_Det_Pt);    ;;;���ճ�������ط�Ӧ����yx_buf0
               }
              *Core_Det_Pt     = yx_buf0;
               Core_Src_Pt_B   = Core_Src_Pt_B + 2;
               Core_Temp_Loop -= 2;
               Core_Src_Len    ++;
          }

          Core_Det_Pt++;         ;;;�������ǹ̶���32·ң�ŵ�,����û��Ҳ��0

          if(Core_Temp_ShortInt == 0xaaaa)     ;;;�ж��Ƿ��б仯,�ȱ�ʾ��
          {                                                     ;;;�Ƕ�yx_change[]������д����;���ݳ���2,����ָ��������һλ                             
             *Core_Src_Pt = *Core_Src_Pt / 2;   Core_Src_Pt++; ;;;ȷ����λYX��ƫ����        
             *Core_Src_Pt = *(Core_Det_Pt-2);   Core_Src_Pt++; ;;;��λYX֡��YXֵ��ÿ֡2���֣�WORD0
             *Core_Src_Pt = *(Core_Det_Pt-1);   Core_Src_Pt++; ;;;��λYX֡��YXֵ��ÿ֡2���֣�WORD1   ��¼������һ�ε�YXֵ
             *Core_Src_Pt = chg_buf0;           Core_Src_Pt++; ;;;��chg_bit0��Ӧ��λֵΪ1�����ʾ��λYX������λ
             *Core_Src_Pt = chg_buf1;
              yx_chg_in_pt ++;             ;;;�Զ��ۼӼĴ������ڱ����λYX֡�ĸ���
              yx_chg_in_pt &= 0xff;        ;;;ȷ��ѭ�� 
              Core_Temp_ShortInt = 0x5555; ;;;����¼�����˾�û�б�Ҫ���ظ���¼�˾͸�5555
          }
      }
  }
}























void core_insert_SOECOS()
{
   // before insert SOE & COS, must fill YX_State with New YX value to generate COS
   
   Core_Det_Pt = (unsigned short *)&yx_event[yx_soe_in_pt].soe_ms;
  //ms
   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   Core_Src_Pt_B += 2;
   Core_Det_Pt++;
  //sec min
   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   Core_Src_Pt_B += 2;
   Core_Det_Pt++;
  //hour day
   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   Core_Src_Pt_B += 2;
   Core_Det_Pt++;
  // channel state

   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   Core_Temp_ShortInt=((*Core_Det_Pt) & 0x87ff) + (unit_info[Core_Src_Unit].yx_start*16 & 0x0fff);
   *Core_Det_Pt = Core_Temp_ShortInt;

   yx_soe_in_pt++;
   if(yx_soe_in_pt>1023)
     yx_soe_in_pt = 0;

   Core_Temp_ShortInt&=0x0fff;
   Core_Temp_Loop=Core_Temp_ShortInt/16;
   Core_Det_Pt = (WORD *)&YX_State[Core_Temp_Loop];
   Core_Src_Pt = &yx_change[yx_chg_in_pt].offset_no;
  *Core_Src_Pt = Core_Temp_Loop/2;           Core_Src_Pt++; // offset
   if(((Core_Temp_Loop)& 0x01)==0)
   {
       *Core_Src_Pt = *(Core_Det_Pt+0x00);   Core_Src_Pt++; // YX0
       *Core_Src_Pt = *(Core_Det_Pt+0x01);   Core_Src_Pt++; // YX1
       *Core_Src_Pt = (WORD)(1<<(Core_Temp_ShortInt%16));    Core_Src_Pt++;   // CHG0
       *Core_Src_Pt = 0x00;                                 // CHG1
   } 
   else
   {
       *Core_Src_Pt = *(Core_Det_Pt-0x01);   Core_Src_Pt++; // YX0
       *Core_Src_Pt = *(Core_Det_Pt+0x00);   Core_Src_Pt++; // YX1
       *Core_Src_Pt = 0x00;                  Core_Src_Pt++; // CHG0
       *Core_Src_Pt = (WORD)(1<<(Core_Temp_ShortInt%16));   // CHG1
   }
   
   yx_chg_in_pt ++;
   yx_chg_in_pt &= 0xff;         
}

void core_insert_SOE()
{
   Core_Det_Pt = (unsigned short *)&yx_event[yx_soe_in_pt].soe_ms;
  //ms
   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   Core_Src_Pt_B += 2;
   Core_Det_Pt++;
  //sec min
   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   Core_Src_Pt_B += 2;
   Core_Det_Pt++;
  //hour day
   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   Core_Src_Pt_B += 2;
   Core_Det_Pt++;
  // channel state

   *Core_Det_Pt = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
   *Core_Det_Pt = ((*Core_Det_Pt) & 0x87ff) + (unit_info[Core_Src_Unit].yx_start*16 & 0x0fff);

   yx_soe_in_pt++;
   if(yx_soe_in_pt>1023)		//-���Ƕ�Ӧ�Ŀռ��¼���յ���IED���͵�ֵ
     yx_soe_in_pt = 0;
}

void core_update_YM()
{
  Core_Temp_Loop = unit_info[Core_Src_Unit].ym_num;		//-���п�����Щʵ�ʹ��ܶ���������Ŷ
  if(*Core_Src_Pt_B>Core_Temp_Loop)			//-�����Ǻ�ʹ��Ĺ���,�������кܳ�һ��ʱ���ʹ��׶�,�����ֵ����,�������Լ���Ŀ��
     return;
  
  Core_Det_Pt = (unsigned short *)&(YM_State[unit_info[Core_Src_Unit].ym_start+(*Core_Src_Pt_B)]);
  if(Core_Temp_Loop<(Core_Src_Len+(*Core_Src_Pt_B)))
     Core_Temp_Loop = Core_Temp_Loop - *Core_Src_Pt_B;
  else
     Core_Temp_Loop = Core_Src_Len;

  Core_Temp_Loop = Core_Temp_Loop*2;
  Core_Src_Pt_B  ++;
  for(Core_Temp_Loop1=0;Core_Temp_Loop1<Core_Temp_Loop;Core_Temp_Loop1++)
  {
     *Core_Det_Pt    = (*Core_Src_Pt_B) + (*(Core_Src_Pt_B+1))*256;
      Core_Src_Pt_B += 2;
      Core_Det_Pt   ++;
  }
    
}