dolbyӦ�ó���ʹ��˵����
Դ�룺dolby_main.c dolby_ir.c dolby_win.c dolby_dvbplay.c dolby_service.c
���ܣ�Ϊ�ű���֤�ṩ����֧�֡�
ִ�з�ʽ������dolbyĿ¼��ʹ��./dolby����ִ�иó���
ʹ��˵����
    1����Ӧ�ó��������󣬻�ֱ�ӽ��뵥Ƶ����̨���ý��档��̨�������Ϊ������Ƶ�ʣ������ʣ����Ʒ�ʽ��Tuner���͡�
        1) �������ƣ�
            (1)ң����"UP"��"DOWN"���������л�����ؼ���
			(2)"LEFT"��"RIGHT"������ѡ�񽹵�����Ҫ�趨�Ĳ�����
			(3)"OK"�����������ƿؼ��༭���ı༭״̬��������������ʼ�����¼���
	 	2) ����������
            (1)����Ƶ�ʣ�Ĭ�ϴ��ڽ���λ�ã�����ok������ʹ�����ɱ༭״̬��������Ӧ�����ּ���������Ƶ�ʡ�������ɺ󣬰���ok����ʹ����Ƶ�ʱ༭�����ڲ��ɱ༭״̬��ע������Ƶ��ֵ��Ϊ0~999��������Ƶ�ʲ�����λ������ǰ���0��
	 		(2)�����ʣ�ֵ��Ϊ0~9999���趨��ʽ������Ƶ����ͬ��
        3) ���Ʒ�ʽ��
		    ��5�����͹�ѡ��16, 32, 64, 128, 256, 512�������Ҽ��ɽ��е��Ʒ�ʽ���͵��л���
        4) Tuner���ͣ�
		    ��21�����͹�ѡ��
			HI_UNF_TUNER_DEV_TYPE_XG_3BL,               /*����Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_CD1616,               /*CD1616��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE,            /*ALPS_TDAE��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_TDCC,                 /*TDCC��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_TDA18250,             /*TDA18250��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_CD1616_DOUBLE,        /*CD1616��˫agc��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_MT2081,               /*MT2081��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_TMX7070X,             /*THOMSON7070X��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_R820C,                /*R820C��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_MXL203,               /*MXL203��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_AV2011,               /*AV2011��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_SHARP7903,            /*SHARP7903��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_MXL101,               /*MXL101��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_MXL603,               /*MXL603��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_IT9170,               /*IT9170��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_IT9133,               /*IT9133��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_TDA6651,              /*TDA6651��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_TDA18250B,            /*TDA18250B��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_M88TS2022,            /*M88TS2022��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_CXD2861,				/*CXD2861��Ƶͷ*/
			HI_UNF_TUNER_DEV_TYPE_RDA5815,              /*RDA5815��Ƶͷ*/
		
			ʹ�����Ҽ����������л���
			�����趨��ɺ󣬽�����������start������ok�������Ŀ�������̡�
	 2����Ŀ��ʼ���ź�����Ļ�·�����һ��Channel Bar������ʾ��Ŀ��Ϣ����������Ϊ����Ƶ���ͣ���������ͣ��������ͣ�������С����Ƶ���͡�
	 	���Ź����а������ƣ�
     	    1) "UP"��"DOWN"����̨��
     	    2) "LEFT"��"RIGHT"���������ƣ������С�������Ҽ�����������
     	    3) "MUTE"�������趨��
     	    4) "AUIDO"�������趨��
     	    5) "EPG"��HDMI��SPDIF͸������
     	 	    ͸����������У�
     	 			a: HDMI�� ��ѡ��AUTO(����HDMI EDID�ж��Ƿ�͸��),LPCM(����),RAW(͸��)��Ĭ��AUTO��
     	 			b: SPDIF����ѡ��LPCM(����),RAW(͸��)��Ĭ��RAW��
     	    6) "BACK"����Ŀ����ʾ������ָʾ��ǰ�����ĸ�Ƶ���Ľ�Ŀ��
     	    7) "SEARCH": ������̨���ý��棬���¿�ʼ��̨��
     	    8) "DEL"������channelBar����ʾ����ʧ��
     	    9) '0~9'�������趨Ҫ���ŵĽ�Ŀ�š���Ŀ�źϷ�ʱ��������̨�����Ϸ�ʱ���������ŵ�ǰ��Ŀ������̨��