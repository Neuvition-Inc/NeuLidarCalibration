#pragma once

enum enPTZ_State {
	PTZ_DEFAULT = -1, //Ĭ�ϵ�״̬  --ɾ������Ҫ���
	PTZ_NO_OPEN = 0, //��̨û�д�
	PTZ_SERIALPORT_CONNECT_SUCESS, //��̨�������ӳɹ�
	PTZ_SERIALPORT_CONNECT_FAIL,//��̨��������ʧ��
	PTZ_STOP, //��̨û�����ƶ�  --�����ÿ��ƣ���Ϊ���ܳ��ִ��ҵ������_waitForNextCmd();  parseReadInfo()�������ط�
	PTZ_MOVE //��̨�����ƶ�
};
class CPTZStateManage
{
public:
	CPTZStateManage() {};
	~CPTZStateManage() {};

	enPTZ_State getPTZState() 
	{
		return m_curPTZState;
	}
	void setPTZState(enPTZ_State curPTZState)
	{
		m_curPTZState = curPTZState;
	}
private:
	enPTZ_State  m_curPTZState = PTZ_NO_OPEN;
};

