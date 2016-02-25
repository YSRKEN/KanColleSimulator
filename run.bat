echo "通常モード その1" > log.txt
KanColleSim 1 fleets1.txt fleets2.txt 単縦陣 単縦陣 >> log.txt
echo "" >> log.txt
echo "通常モード その2" >> log.txt
KanColleSim 10000 fleets1.txt fleets2.txt 単縦陣 単縦陣 >> log.txt
echo "" >> log.txt
echo "マップモード その1" >> log.txt
KanColleSim 1 fleets3.txt 1-5_high.map 梯形陣 マップ >> log.txt
echo "" >> log.txt
echo "マップモード その2" >> log.txt
KanColleSim 10000 fleets3.txt 1-5_high.map 梯形陣 マップ >> log.txt
echo "" >> log.txt
echo "マップモード その3" >> log.txt
KanColleSim 1 fleets4.txt 3-5削り_上_high.map 梯形陣 マップ >> log.txt
echo "" >> log.txt
echo "マップモード その4" >> log.txt
KanColleSim 10000 fleets4.txt 3-5削り_上_high.map 梯形陣 マップ >> log.txt
echo "マップモード その5" >> log.txt
KanColleSim 1 fleets6.txt 2-5削り_上_high.map 梯形陣 マップ >> log.txt
echo "" >> log.txt
echo "マップモード その6" >> log.txt
KanColleSim 10000 fleets6.txt 2-5削り_上_high.map 梯形陣 マップ >> log.txt
pause
