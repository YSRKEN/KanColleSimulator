sum1 = Array.new(720, 0)
sum2 = Array.new(720, 0)
1.upto(8){|i|
	File.read("log_#{i}.txt").split("\n").each_with_index{|line, index|
		next if index >= 720
		temp = line.split(",")
		sum1[index] += temp[1].to_i
		sum2[index] += temp[2].to_i
	}
}
output = ""
720.times{|i|
	output << "#{sum1[i]}\t#{sum2[i]}\n"
}
File.write("log_sum.txt", output)
