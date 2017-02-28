/**
* @file errorcode.c
* @brief
* @author rgw
* @version v1.0
* @date 2017-02-15
*/
#include "includes.h"
#include "errorcode.h"

//
//  һ��Ҫ��֤code�� ˳�� ��ͷ�ļ�����һ��
//
const uint8_t *strErrorCode[100] =
{
    "�޹���",
    "��ͣ��ⷢ������",
    "�������ٶȴ�������������",
    "�����ӵ������Դ�������������",
    "����״̬�������������",
    "��������������",

    "���� ���ͨ�Ź���",
    "���� ���CP����",
    "���� ���CC����",
    "���� ��ǹ״̬�������������",
    "���� B������ǹ��״̬�������������",
    "���� B������ǹ���޷���ֹ",
    "���� L�����¶ȼ�ⷢ������",
    "���� N�����¶ȼ�ⷢ������",
    "���� B�������¶ȼ���1��������",
    "���� B�������¶ȼ���2��������",
    "���� ��ʼ������",
    "���� ����������",

    "�ļ���д����",
    "��Ŀ���ļ�",
    "��������",
    "����������",
    "������������żУ�����",
    "������������������",
    "����������֡����",
    "���������ڳ���",
    "����������DMA�������",
    "����ģ�鴮����żУ�����",
    "����ģ�鴮����������",
    "����ģ�鴮��֡����",
    "����ģ�鴮�ڳ���",
    "����ģ�鴮��DMA�������",
    "����ͨ�Ź���"
};

void ThrowErrorCode(uint32_t ulDevID, ErrorCode_t errcode, ErrorLevel_t errlevel)
{
    ErrorPackage_t package;

    package.ulDevID = ulDevID;
    package.code = errcode;
    package.level = errlevel;

    xQueueSend(xHandleQueueErrorPackage, (void *)&package, 0);
}
