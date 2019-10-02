#include "types.h"
#include "x86.h"
#include "defs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "traps.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"

/*struct circular_buffer {
    char bytes[12];
    char *head;
    char *tail;
    int count;
};*/

//static struct circular_buffer cbuff;
static struct spinlock mouselock;
static struct sleeplock mouse_sleeplock;
int bytes[100];
int count;
int read_index;
int write_index;

// num = 0 is for reading
// num = 1 is for writing
void mouse_poll_loop(int num) {
    uint counter = 0;
    if(num == 0)
        while(counter < 100 || (inb(0x64) & 0x01) == 0) { counter++; }
    else
        while(counter < 100 || (inb(0x64) & 0x02) == 1) { counter++; }
}

void consume() {
    mouse_poll_loop(0);
    uint ack;
    do {
        ack = inb(0x60);
    } while (ack != 0xFA);
}

void bufferinit() {
/*    cprintf("Address of bytes[0]: %p\n", &(cbuff.bytes[0]));
    cbuff.head = &(cbuff.bytes[0]);
    cprintf("Address of head: %p\n", cbuff.head);
    cprintf("Address of bytes[11]: %p\n", &(cbuff.bytes[11]));
    cbuff.tail = &(cbuff.bytes[11]);
    cprintf("Address of tail: %p\n", cbuff.tail);*/
    count = 0;
    read_index = 0;
    write_index = 0;
}

int mouse_syscall_helper() {
    acquiresleep(&mouse_sleeplock);
    acquire(&mouselock);
    while(count == 0) {
        release(&mouselock);
        if(myproc()->killed) {
            releasesleep(&mouse_sleeplock);
            exit();
        }
        acquire(&mouselock);
    }
    int packet = remove();
//    cprintf("packet: %d\n", packet);
    release(&mouselock);
    releasesleep(&mouse_sleeplock);
    return packet;
}

void insert(int data, int data2, int data3) {
    // if (holdingsleep(&mouse_sleeplock) == 1)
    //     releasesleep(&mouse_sleeplock);
    if (count == 100) {
        //cprintf("%s\n", "Sorry, buffer is currently full - discarding new data...");
        return;
    }
    if ((data & 0x20) == 1)
        data3 |= 0xFFFFFF00;
    if ((data & 0x10) == 1)
        data2 |= 0xFFFFFF00;
    //cprintf("1st: %d  2nd: %d   3rd: %d\n", data, data2, data3);
    int pkt = (data << 16) | (data2 << 8) | data3;
    bytes[write_index++] = pkt;
    count++;
    if (write_index == 100)
        write_index = 0;
}

int remove() {
    if (count == 0) {
        // acquiresleep(&mouse_sleeplock);
        // cprintf("%s\n", "Sorry, buffer is empty");
        return -1;
    }
    int pkt = bytes[read_index++];
    count--;
    if (read_index == 100)
        read_index = 0;
    return pkt;
}

void mouseinit() {

    // BEFORE YOU WRITE MAKE SURE READ FROM PORT 64 AND VERIFY BIT 1 IS CLEAR
    // BEFORE YOU READ MAKE SURE READ FROM PORT 64 AND VERYIFY BIT 0 IS SET

    uint stb;

//    mouse_poll_loop(1);
//    outb(0x64, 0xa8);

    mouse_poll_loop(1);
    outb(0x64, 0x20); // issue the get compaq status byte

    mouse_poll_loop(0);

    stb = inb(0x60); // get the status byte returned
//    cprintf("%d", stb);
    stb |= 0x02; // set bit 1
    stb &= ~(1 << 5); // clear bit 5
 //   cprintf("%d", stb);

    mouse_poll_loop(1);
    outb(0x64, 0x60); //issue the set compaq status byte

    mouse_poll_loop(1);
    outb(0x60, stb); // send the modified byte to port 0x60

    mouse_poll_loop(1); //issue command 0xd4 to ps/2 controller then f4 to activate mouse
    outb(0x64, 0xd4);
    mouse_poll_loop(1);
    outb(0x60, 0xf4);
    consume();

    initlock(&mouselock, "mouse");
    initsleeplock(&mouse_sleeplock, "mousesleep");
    ioapicenable(IRQ_MOUSE, 0);

    bufferinit();

 //   cprintf("%s\n", "helloya");
}

void mouseintr() {
//    cprintf("%s", "Got mouse interrupt\n");
    acquire(&mouselock);
    int stb;

    while (((stb = inb(0x64)) & 1) == 1) {
        int data1 = inb(0x60);
        while ((data1 & 0x8) == 0) { data1 = inb(0x60); }
        int data2 = inb(0x60);
        int data3 = inb(0x60);
        // cprintf("first byte: %d\n", data1);
        // cprintf("second byte: %d\n", data2);
        // cprintf("third byte: %d\n", data3);
        if ((data1 & 0x80) == 0 && (data1 & 0x40) == 0)
            insert(data1, data2, data3);
        // cprintf("first byte: %d\n", remove());
        // cprintf("second byte: %d\n", remove());
        // cprintf("third byte: %d\n", remove());
        break;
    }

    release(&mouselock);
//    cprintf("%s\n", "Helloya?");
}