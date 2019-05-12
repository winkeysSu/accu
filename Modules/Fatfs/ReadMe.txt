1.总体说明：
在Fatrfs原有基础上，参考stm32cube对其的封装，加入了新的接口代码，文件如下：

documents Fatfs原有的说明文档，00index_e.html可以浏览查看，主要看Fatfs的移植，配置和API使用说明

source    Fatfs的源代码，内部的00readme.txt文件有对各个文件的说明。

user      新加的一层封装，其中：
	fatfs.c，fatfs.h  -  对Fatfs的初始化调用
	ff_gen_drv.c, ff_gen_drv.h   -  连接fatfs和用户驱动的功能,FATFS_LinkDriver()起到该作用
	user_diskio.c, user_diskio.h  -  用户实现的驱动，这里可以根据需要，加入不同设备的驱动

2.移植
Fatfs的驱动在disk.c文件中，经过封装整理后，我们可以在user_diskio.c和user_diskio.h中实现具体的驱动，或者另外编写驱动文件,而后通过FATFS_LinkDriver()连接到Fatfs中
如果需要加锁等功能，需要在ffsystem.c中实现

3.配置
Fatfs的配置在ffconf.h中实现具体的驱动

4.补充说明
本代码中已经实现了基于FreeRtos的配置，用宏定义FF_USE_FREERTOS使能
  本代码中实现了W25Q64作为媒介的文件系统，可参考
  注意：disk_read(), disk_write(),disk_ioctl()里面用到的sector都是指扇区的索引地址，而不是byte偏移地址；
		disk_write()中不能只实现对扇区的写入，必须实现对起的有效写入（先保证擦除完毕，再写入）；	disk_ioctl()cmd为CTRL_TRIM时，实现擦除相应扇区；调用f_mkfs()时会执行到这里，由于一个个扇区擦除会很滑时间，所以建议可以特别优化
