echo "通常モード その1" > log.txt
KanColleSim 1 sample\fleets1.txt sample\fleets2.txt 単縦陣 単縦陣 >> log.txt
echo "" >> log.txt
echo "通常モード その2" >> log.txt
KanColleSim 110000 sample\fleets1.txt sample\fleets2.txt 単縦陣 単縦陣 >> log.txt
echo "" >> log.txt
echo "マップモード その1" >> log.txt
KanColleSim 1 sample\fleets4.txt sample\3-5削り_上_high.map 梯形陣 マップ >> log.txt
echo "" >> log.txt
echo "マップモード その2" >> log.txt
KanColleSim 110000 sample\fleets4.txt sample\3-5削り_上_high.map 梯形陣 マップ >> log.txt
pause
