objects=imu_data_decode.o packet.o imu_serial.o
#objects=serial.o

imu:$(objects)
	cc -o imu $(objects)

imu_date_decode.o:imu_data_decode.c imu_data_decode.h packer.h
	cc -c imu_data_decode.c

packer.o:packet.c packer.h
	cc -c packet.c

serial.o:imu_serial.cpp imu_data_decode.h packet.h
	cc -c imu_serial.cpp

clean:
	rm imu $(objects)
