

asm196  cstart.a96    md(sa-ef) cmain ;;;�������ָ����Զ�CSTART.A96���б�������Ŀ���ļ�

c196    reg.c         md(sa-ef) code xref ms ;;;����Ķ������һ����������
c196    ram.c         md(sa-ef) code xref ms
c196    rom.c         md(sa-ef) code xref ms
        �ļ���         ѡ������ָ�     ���б��ļ��в����������б�   ���б��ļ��в���������Ų����б�    ������ϻ��Դ�ļ�
c196    main.c        md(sa-ef) code xref ms
c196    int.c         md(sa-ef) code xref ms ip(0xfb)
c196    port.c        md(sa-ef) code xref ms 
c196    core.c        md(sa-ef) code xref ms 
c196    comuse.c      md(sa-ef) code xref ms 
c196    protocol.c    md(sa-ef) code xref ms 

c196    cdt.c         md(sa-ef) code xref ms 
c196    cdt9702.c     md(sa-ef) code xref ms 
c196    hostzf.c      md(sa-ef) code xref ms 
c196    set.c         md(sa-ef) code xref ms
c196    bus.c         md(sa-ef) code xref ms
c196    can.c         md(sa-ef) code xref ms
c196    zlp_cdt.c     md(sa-ef) code xref ms 
c196    rec_host.c    md(sa-ef) code xref ms
c196    DS_XXQ.c      md(sa-ef) code xref ms
c196    bjsf103.c     md(sa-ef) code xref ms
c196    ddb_bb.c      md(sa-ef) code xref ms 
c196    ddb_ws.c      md(sa-ef) code xref ms 
c196    XDL_MLX.c     md(sa-ef) code xref ms
c196    zpjk.c        md(sa-ef) code xref ms
c196    zlp_zwk.c     md(sa-ef) code xref ms 
c196    zlp_js.c      md(sa-ef) code xref ms 

rl196 -f alo

;;;�Ͼ�����ÿ��ܾ�������ALO��ȥִ����Щ����,����
;-----------------------------------------------------
cstart.obj     main.obj     reg.obj     ram.obj       int.obj 
port.obj       core.obj     comuse.obj  protocol.obj  rec_host.obj  cdt.obj
hostzf.obj     set.obj      zlp_js.obj  bus.obj       can.obj       ds_xxq.obj     XDL_MLX.obj    zpjk.obj      
ddb_ws.obj     rom.obj      zlp_zwk.obj ddb_bb.obj    bjsf103.obj   CDT9702.obj    ZLP_CDT.obj   ;;;��Щ���ǽ�������֮�����ֱ�������õ��ļ�,�����й�Լ�ڴ�(���õĿ�ɾ)

c96.lib to main.abs 
md(sa-ef) bu noit 
ram(0002000H-000f7ffH,000f800H-000ffffH(stack),0010000H-003ffffH,0f40000H-0f6ffffH)                                                &
rom(0f80000H-0f9ffffH,0fa0000H-0faffffH(cdt,hostzf,can),0fb0800H-0fbffffH(main,int,port,core,comuse,set,protocol,bus))
ss(2048) sfr noregoverlay
;-----------------------------------------------------

oh196 main.abs        ;;;ת����HEX�ļ�




rem c196    printf.c    md(sa-ef) code xref ms   ;;;�������ļ���û���õ�,���л���Ҫ�������������