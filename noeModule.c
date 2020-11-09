#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>

/*Cesar*/

#define LONGITUD_ALFABETO 26
#define INICIO_ALFABETO_MAYUSCULAS 65
#define INICIO_ALFABETO_MINUSCULAS 97

#define MOD(i, n) (i % n + n) % n

const char *alfabetoMinusculas = "abcdefghijklmnopqrstuvwxyz",
           *alfabetoMayusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Recibe un mensaje a cifrar y un búfer en donde pondrá el resultado, así como
// las rotaciones que debe dar
void encryptWithCesar(const char *mensaje, char *destino, int length);
int ord(char c);

/*Cesar*/

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "noedevice" /* Dev name as it appears in /proc/devices  */
#define BUF_LEN 1024            /* Max length of the message from the device */
#define INDEX_CESAR 4
/*Global variables are declared as static, so are global within the file. */
static int Major;
static int MajorSelected = 0;
/* Major number assigned to our device driver */
static int Device_Open = 0;
/* Is device open? Used to prevent multiple access to device */
static char msg[BUF_LEN];
static unsigned long bytes_to_write = 0;
/* The msg the device will give when asked */
static char *msg_Ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release};

/* * This function is called when the module is loaded */
int init_module(void)
{
  Major = register_chrdev(MajorSelected, DEVICE_NAME, &fops);

  if (Major < 0)
  {
    printk(KERN_ALERT "Registering char device failed with %d\n", Major);
    return Major;
  }
  printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
  printk(KERN_INFO "the device, create a dev file with\n");
  printk(KERN_INFO "'sudo mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
  printk(KERN_INFO "then give the device write and read permissions\n");
  printk(KERN_INFO "sudo chmod 666 /dev/%s \n", DEVICE_NAME);
  printk(KERN_INFO "then write the device with echo 'message'> /dev/%s \n", DEVICE_NAME);
  printk(KERN_INFO "and read  with cat /dev/%s \n", DEVICE_NAME);
  return SUCCESS;
}

/*This function is called when the module is unloaded */
void cleanup_module(void)
{
  /*Unregister the device */
  unregister_chrdev(MajorSelected, DEVICE_NAME);

  printk(KERN_INFO "UNGS : Driver Noelia No registrado\n");
}
/* Methods */

/* Called when a process tries to open the device file, like "cat /dev/mycharfile" */

static int device_open(struct inode *inode, struct file *file)
{
  //static int counter = 0;

  if (Device_Open)
    return -EBUSY;

  Device_Open++;
  //sprintf(msg);
  msg_Ptr = msg;
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

/*  * Called when a process closes the device file. */
static int device_release(struct inode *inode,
                          struct file *file)
{

  Device_Open--; /* We're now ready for our next caller */

  /* Decrement the usage count, or else once you opened the file, you'll never get get rid of the module. */
  module_put(THIS_MODULE);

  return 0;
}

/* Called when a process, which already opened the dev file, attempts to read from it. */
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */ char *buff, /* buffer to fill with data */ size_t length, /* length of the buffer     */ loff_t *offset)
{
  int count = 0;
  while (length && *msg_Ptr)
  {
    put_user(*(msg_Ptr++), buff++);
    length--;
    count++;
  }
  return count;
}

/* Called when a process writes to dev file: echo "hi" > /dev/hello  */
static ssize_t device_write(struct file *fp,
                            const char *buff,
                            size_t length, loff_t *ppos)
{
  bytes_to_write = length;
  if (bytes_to_write > BUF_LEN)
  {
    bytes_to_write = BUF_LEN;
  }
  
  if (copy_from_user(msg, buff, bytes_to_write))
  {
    return -EFAULT;
  }
  msg[bytes_to_write] = 0;
  encryptWithCesar(buff, msg, bytes_to_write);
  printk(KERN_INFO "noedevice : mensaje encriptado %s\n", msg);
  return bytes_to_write;
}

void encryptWithCesar(const char *mensaje, char *destino, int length)
{

  /*Recorrer cadena*/
  int i = 0;
  while (i <= length)
  {

    char caracterActual = mensaje[i];
    int posicionOriginal = ord(caracterActual);

    if (!isalpha(caracterActual))
    {
      /*Si no es letra no lo modifico*/
      destino[i] = caracterActual;
      i++;
      continue;
    }

    if (isupper(caracterActual))
    {
      /*Me fijo si es mayuscula o no*/
      destino[i] =
          alfabetoMayusculas[(posicionOriginal - INICIO_ALFABETO_MAYUSCULAS +
                              INDEX_CESAR) %
                             LONGITUD_ALFABETO];
    }
    else
    {

      destino[i] =
          alfabetoMinusculas[(posicionOriginal - INICIO_ALFABETO_MINUSCULAS +
                              INDEX_CESAR) %
                             LONGITUD_ALFABETO];
    }
    i++;
  }
}

int ord(char c) { return (int)c; }

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UNGS");
MODULE_DESCRIPTION("Un primer driver");
