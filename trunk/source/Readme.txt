PDB Explorer �����ĵ�

�汾��v1.10.0217(Unicode)
����ƽ̨��Win2K/WinXP
��Ȩ���У�Titi ������
���ߣ�����
�����ʼ���titilima@163.com
������ҳ��http://www.titilima.cn

���
PDB Explorer ��һ���ܹ��鿴΢�� pdb �ļ���Program DataBase ���������ݿ⣩�Ĺ��ߣ����ܹ��� pdb �ļ��е� struct ��union �� enum ���͵Ķ����� C/C++ ���﷨��ʾ�������ر��ʺ� Windows �ײ��о���Ա�� DDK ����Աʹ�á�
��ע�⣬��ʹ�� PDB Explorer ֮ǰ����ȷ�����Ѿ�ע����΢��� DIA �⣨msdia71.dll �� msdia80.dll�������� PDB Explorer ���������С�

FAQ
Q: ����Զ��� PDB Explorer ���﷨����ɫ��
A: ���� PDBExp.htm ����ʽ�༭һ�� HTML �ļ���Ȼ���ڡ����á��н����༭�õ� HTML �ļ���Ϊģ�� HTML �ļ�������������� PDB Explorer ���ɡ�
Q: ��������š���ʲô�ã�
A: ������ڿ���ʱҪʹ��һ���ϴ�Ľṹ���� _EPROCESS �� _PEB����������ʹ�á�������š�����ѡ��������õ� struct ��Ա�� enum �PDB Explorer �Ὣ��������û�õ���Ŀ�� UCHAR ���ֽڣ��������ʽ����ʡ�ԡ�����ʾ���˾�������� _EPROCESS �ṹ��WinXP����
    struct _EPROCESS // 0x260
    {
        UCHAR uUseless0[0x138]; // +0x0(0x138)
        void* SectionObject; // +0x138(0x4)
        UCHAR uUseless1[0x38]; // +0x13c(0x38)
        UCHAR ImageFileName[0x10]; // +0x174(0x10)
        UCHAR uUseless2[0x2c]; // +0x184(0x2c)
        struct _PEB* Peb; // +0x1b0(0x4)
        UCHAR uUseless3[0x94]; // +0x1b4(0x94)
        ULONG Flags; // +0x248(0x4)
        UCHAR uUseless4[0x18]; // +0x248(0x18)
    };
Q: struct ��Ա���ע����ʲô��˼��
A: ��һ����ֵ�Ǹó�Ա��� struct ��ƫ�ƣ��ڶ�����ֵ�Ǹó�Ա�Ĵ�С��

���¼�¼

* 2009.07.27 - 1.10.0217
  �����˶� typedef �� class ��֧�֡�
  ���� PDB �����˶� NB10 ǩ����֧�֡�
  ������ö��������ʾö��ֵ����� bug��
  ����������Ӱ���û������ bug��
* 2009.04.15 - 1.03.039
  �Ż��˴���Ĵ�С��
  �Ż������� PDB �ļ����ܵ��û����顣
  �����˴� PE ӳ����ȡ PDB �ļ����� bug��
* 2008.11.20 - 1.02.035
  �������� PDB �ļ��Ĺ��ܡ�
  ��� DIA ��汾�����ݵ����⡣
  �������ļ��ظ��Ϸŵ��³�������� bug �� 
* 2007.12.21 - 1.01.025
  �Ż��˼����ٶȣ�������ļ��Ϸŵ�֧�֡� 
* 2007.11.13 - 1.00.023
  ������
