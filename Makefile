all: diningPhilosopher ReaderWriter sleepingBarbers smokers SushiBar UnisexBathroom PC_BoundedBuffer PC_CyclicBuffer

diningPhilosopher: diningPhilosopher.c
	gcc diningPhilosopher.c -o diningPhilosopher -lpthread

ReaderWriter: ReaderWriter.c
	gcc ReaderWriter.c -o ReaderWriter -lpthread

sleepingBarbers: sleepingBarbers.c
	gcc sleepingBarbers.c -o sleepingBarbers -lpthread

smokers: smokers.c
	gcc smokers.c -o smokers -lpthread

SushiBar: SushiBar.c
	gcc SushiBar.c -o SushiBar -lpthread

UnisexBathroom: UnisexBathroom.c
	gcc UnisexBathroom.c -o UnisexBathroom -lpthread

PC_BoundedBuffer: PC_BoundedBuffer.c
	gcc PC_BoundedBuffer.c -o PC_BoundedBuffer -lpthread

PC_CyclicBuffer: PC_CyclicBuffer.c
	gcc PC_CyclicBuffer.c -o PC_CyclicBuffer -lpthread

clean:
	rm -f diningPhilosopher ReaderWriter sleepingBarbers smokers SushiBar UnisexBathroom PC_BoundedBuffer PC_CyclicBuffer
