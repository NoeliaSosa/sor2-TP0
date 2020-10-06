# Trabajo Practico Inicial de Sistemas Operativos y Redes II
## Device Drivers - Char Device

#### PASO 1

###### Escribir un archivo Makefile para realizar la compilacion y creacion del device

obj -m := miModulo .o

all :
    make -C / lib/ modules /$( shell uname -r) / build SUBDIRS =$( shell pwd ) modules 
clean :
    make -C / lib/ modules /$( shell uname -r) / build SUBDIRS =$( shell pwd ) clean
    
   
 #### PASO 2 
 
 ##### Crear archivo .c con la implementacion de los metodos mencionados a continuación:
 
int init_module(void); 
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

 #### PASO 3
 ##### En la carpeta en la que estan alojados los archivos ejecutar los siguientes comandos:
 ~
make 
sudo insmod nombrearchivo.ko
 ~
  #### PASO $
 ##### Posicionar en /dev crear device:
 ~
 sudo mknod nombredeldevice c numeroqueseledioamajor 0
 sudo chmod 666 nombredeldevice
~

  #### PASO 4
  ###### Escribir en el device "echo "mensaje" > /dev/nombredeldevice"
  ###### Lear el device "cat /dev/nombredeldevice"
