# Trabajo Práctico Inicial de Sistemas Operativos y Redes II
## Device Drivers - Char Device

#### PASO 1

 ##### Escribir un archivo Makefile para realizar la compilación y creación del device
 
```
obj -m := miModulo .o

all :
    make -C / lib/ modules /$( shell uname -r) / build SUBDIRS =$( shell pwd ) modules 
clean :
    make -C / lib/ modules /$( shell uname -r) / build SUBDIRS =$( shell pwd ) clean
  ```  
   
 #### PASO 2 
 
 ##### Crear archivo .c con la implementación de los siguientes métodos:
 
``` 
int init_module(void); 
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
```
 #### PASO 3
 
 ##### Para compilar el código y registrar el modulo 
 ##### Posicionarse en la carpeta en la que estan alojados los archivos, ejecutar los siguientes comandos:
 
```
make 
sudo insmod nombrearchivo.ko
```

#### PASO 4 
  
##### Buscar el valor del Major asignado en el log del kernel con el siguiente comando:
  
```
dmesg

```

 #### PASO 5
  
 ##### Para registrar el device y darle premisos de lectura y escritura ejecutar los siguientes comandos: 
 
```
 sudo mknod dev/nombredeldevice c numeroqueseledioamajor 0
 sudo chmod 666 dev/nombredeldevice
```

  #### PASO 6
  
  ###### Escribir en el device con "echo "mensaje" > /dev/nombredeldevice"
  
  ###### Lear el device "cat /dev/nombredeldevice"
