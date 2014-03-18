# use of
#   sox
#   CImg
#   ffmpeg

.phony:all e vid vids vidaudios cleanoutput show backup publish abribus output/dino happy list touch

all: output/abribus/ok.txt output/dino/ok.txt output/happy/ok.txt
#	./spark -s scenes/common.sp -s scenes/meshes.sp -s scenes/abribus.sp -s scenes/dino.sp -s scenes/happy.sp -s scenes/scene.sp

vids: output/abribus/out_a.mp4 output/dino/out_a.mp4 output/happy/out_a.mp4
vidaudios: output/abribus/out_b.mp4 output/dino/out_b.mp4 output/happy/out_b.mp4
output/out.mp4:output/abribus/out_c.ts output/dino/out_c.ts output/happy/out_c.ts
	rm -f $@
	ffmpeg -i "concat:output/abribus/out_c.ts|output/dino/out_c.ts|output/happy/out_c.ts" -c copy -bsf:a aac_adtstoasc output/out.mp4

vid: output/out.mp4

touch:
	touch scenes/common.sp

subdirs:
	mkdir -p output/abribus
	mkdir -p output/dino
	mkdir -p output/happy

output/%/ok.txt: scenes/%.sp spark scenes/common.sp
	rm -rf output/$(*F)
	mkdir -p output/$(*F)
	./spark -s scenes/common.sp -s scenes/meshes.sp -s $< -e play_$(*F)"();"
	touch $@


output/%/out_a.mp4: output/%/ok.txt
	rm -f $@
	ffmpeg -r 25 -pattern_type glob -i output/$(*F)'/image*.png' -c:v libx264 $@

output/%/out_b.mp4: output/%/out_a.mp4
	rm -f $@
	ffmpeg -i $< -i input/getlucky.mp3 -map 0 -map 1 -codec:a aac -strict experimental -b:a 192k -shortest output/$(*F)/out_b.mp4

output/%/out_c.ts: output/%/out_b.mp4
	rm -f $@
	ffmpeg -i $< -c copy -bsf:v h264_mp4toannexb -f mpegts output/$(*F)/out_c.ts



list:
	rm -f imagelist.txt
	printf "file '%s'\n" output/abribus/* >> imagelist.txt
	printf "file '%s'\n" output/dino/* >> imagelist.txt
	printf "file '%s'\n" output/happy/* >> imagelist.txt

cleanoutput:
	rm -f output/image*.* output/*/image*.* output/*.mp3 output/out*.mp4

cleandirs:
	rm -rf output/abribus output/dino output/happy

fast: spark
	./spark -s scenes/common.sp -e 'set waitlength = 0;' -s scenes/scene.sp

raster: spark
	./spark -s scenes/common.sp -e "raster();"

e:
	emacs Makefile spark.cpp&

spunk.o: spunk.cpp
	g++ -g -c spunk.cpp  -Wall -W -Wsign-compare -ansi -pedantic   -lboost_system -lboost_filesystem -std=c++11

spark: spunk.hpp spunk.o spark.cpp ../CImg/CImg-1.5.7/CImg.h #Makefile
	g++ -g -o spark spunk.o spark.cpp -I. -I../CImg/CImg-1.5.7 -Wall -W -Wsign-compare -ansi -pedantic -Dcimg_use_vt100 -Dcimg_use_png -I/usr/X11R6/include  -lm -L/usr/X11R6/lib -lpthread -lX11 -lpng -lboost_system -lboost_filesystem -std=c++11

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
	cp output/out.mp4 ~/host/"fifpark."`date +%Y%m%d.b`.mp4
	ls -ltr ~/host | tail -2
