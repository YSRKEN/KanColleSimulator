echo "�ʏ탂�[�h ����1" > log.txt
KanColleSim 1 fleets1.txt fleets2.txt �P�c�w �P�c�w >> log.txt
echo "" >> log.txt
echo "�ʏ탂�[�h ����2" >> log.txt
KanColleSim 10000 fleets1.txt fleets2.txt �P�c�w �P�c�w >> log.txt
echo "" >> log.txt
echo "�}�b�v���[�h ����1" >> log.txt
KanColleSim 1 fleets3.txt 1-5_high.map ��`�w �}�b�v >> log.txt
echo "" >> log.txt
echo "�}�b�v���[�h ����2" >> log.txt
KanColleSim 10000 fleets3.txt 1-5_high.map ��`�w �}�b�v >> log.txt
echo "" >> log.txt
echo "�}�b�v���[�h ����3" >> log.txt
KanColleSim 1 fleets4.txt 3-5���_��_high.map ��`�w �}�b�v >> log.txt
echo "" >> log.txt
echo "�}�b�v���[�h ����4" >> log.txt
KanColleSim 10000 fleets4.txt 3-5���_��_high.map ��`�w �}�b�v >> log.txt
echo "�}�b�v���[�h ����5" >> log.txt
KanColleSim 1 fleets6.txt 2-5���_��_high.map ��`�w �}�b�v >> log.txt
echo "" >> log.txt
echo "�}�b�v���[�h ����6" >> log.txt
KanColleSim 10000 fleets6.txt 2-5���_��_high.map ��`�w �}�b�v >> log.txt
pause
