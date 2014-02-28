# use of 
#   sox
#   CImg
#   ffmpeg

.phony:all e vid cleanoutput show backup publish

all: spark
	./spark -s scenes/common.sp -s scenes/abribus.sp -s scenes/scene.sp

fast: spark
	./spark -s scenes/common.sp -e 'set waitlength = 0;' -s scenes/scene.sp

dino: spark
	./spark -s scenes/common.sp -e 'var dino = loadmesh("/home/fmaurel/prog/spark/input/dino2.off", color(0, 255, 0), 0.02);rotate(dino);'

abribus: spark
	./spark -s scenes/common.sp -s scenes/abribus.sp -e "abribus();"

happy: spark
	./spark -s scenes/common.sp -s scenes/happy.sp

e:
	emacs Makefile spark.cpp&

spark: spark.cpp ../CImg/CImg-1.5.7/CImg.h #Makefile
	g++ -g -o spark spark.cpp  -I../CImg/CImg-1.5.7 -Wall -W -Wsign-compare -ansi -pedantic -Dcimg_use_vt100 -Dcimg_use_png -I/usr/X11R6/include  -lm -L/usr/X11R6/lib -lpthread -lX11 -lpng -lboost_system -lboost_filesystem -std=c++11

cleanoutput:
	rm -f output/image*.bmp output/*.mp3

vid:
	rm -f output/out*.mp4
	ffmpeg -r 25 -pattern_type glob -i 'output/image*.bmp' -c:v libx264 output/out1.mp4 
	ffmpeg -i output/out1.mp4 -i input/getlucky.mp3 -map 0 -map 1 -codec copy -shortest output/out.mp4 

vidtoimages:
	mkdir -p videoextracts/testvideoiphone
	ffmpeg -i input/testvideoiphone.MOV -r 25 -t 4 videoextracts/testvideoiphone/image-%5d.jpeg

output/dinocut.mp3: Makefile
	sox input/getlucky.mp3 output/dinocut1.mp3 trim 0.5 8
	sox output/dinocut1.mp3 output/dinocut1.mp3 output/dinocut.mp3 splice 8,0.5
#,1
#	sox output/dinocut.mp3 -e stat
# ffmpeg -ss 00:00:30.00 -t 25 -i input/getlucky.mp3 -acodec copy output/dinocut.mp3

show:
	thunar output/out.mp4

backup:
	tar -cvf backup.tar spark.cpp scenes/*.sp Makefile
	cp backup.tar "backups/backup.tar."`date +%Y%m%d%H%M%S`
	tar -tvf "backups/backup.tar."`date +%Y%m%d%H%M%S`
	ls -ltr backups | tail -1

publish:
	cp backup.tar ~/host/"spark.tar."`date +%Y%m%d`
	cp output/out.mp4 ~/host/"fifpark."`date +%Y%m%d`.mp4
	ls -ltr ~/host | tail -2
