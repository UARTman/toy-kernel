#include "idt.h"
#include "third_party/printf/printf.h"
#include "pic.h"

idt_descriptor_t idt_descriptor_encode(idt_descriptor_info_t info)
{
    idt_descriptor_t descriptor;
    descriptor.offset_0_15 = info.offset & 0xFFFF;
    descriptor.offset_16_31 = (info.offset >> 16) & 0xFFFF;

    descriptor.zero = 0;
    descriptor.type_attr = info.type_attr.byte;
    descriptor.selector = *(uint16_t *)(&info.selector);

    return descriptor;
};

idt_descriptor_info_t idt_descriptor_decode(idt_descriptor_t descriptor)
{
    idt_descriptor_info_t info;
    info.offset = descriptor.offset_0_15 | (descriptor.offset_16_31 << 16);
    uint16_t selector = descriptor.selector;
    info.selector = *(idt_selector_t *)&selector;
    info.type_attr.byte = descriptor.type_attr;
    return info;
};

idt_header_t idtr;
idt_descriptor_t idt_table[256];

typedef struct __attribute__((__packed__)) registers {
    uint32_t ds;
    // Pushed by pusha.
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    // Pushed by the processor automatically.
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

/* BLACK MAGIC – strongly discouraged! */
void interrupt_handler()
{
    __asm__("pushal");

    printf("Interrupt!\n");
    __asm__("cli");
    while (1)
    {
        asm("hlt");
    }
    /* do something */
    __asm__("popal; leave; iret"); /* BLACK MAGIC! */
}

idt_descriptor_t idt_isr(void(*ptr)) {
    idt_descriptor_info_t info = {
        .offset = (uint32_t)ptr,
        .selector = {
            .rpl_privilege_level = 0,
            .table_indicator = 0,
            .segment = 1,
        },
        .type_attr = {
            .info = {
                .gatetype = IDT_GATETYPE_TRAP_32,
                .present = 1,
                .privilege_level = 0,
                .storage = 0,
            },
        }};
    return idt_descriptor_encode(info);
}

void idt_init()
{
    idtr.limit = 256 * sizeof(idt_descriptor_t) - 1;
    idtr.base = (uint32_t)&idt_table;

    idt_table[0] = idt_isr(&isr0);
    idt_table[1] = idt_isr(&isr1);
    idt_table[2] = idt_isr(&isr2);
    idt_table[3] = idt_isr(&isr3);
    idt_table[4] = idt_isr(&isr4);
    idt_table[5] = idt_isr(&isr5);
    idt_table[6] = idt_isr(&isr6);
    idt_table[7] = idt_isr(&isr7);
    idt_table[8] = idt_isr(&isr8);
    idt_table[9] = idt_isr(&isr9);
    idt_table[10] = idt_isr(&isr10);
    idt_table[11] = idt_isr(&isr11);
    idt_table[12] = idt_isr(&isr12);
    idt_table[13] = idt_isr(&isr13);
    idt_table[14] = idt_isr(&isr14);
    idt_table[15] = idt_isr(&isr15);
    idt_table[16] = idt_isr(&isr16);
    idt_table[17] = idt_isr(&isr17);
    idt_table[18] = idt_isr(&isr18);
    idt_table[19] = idt_isr(&isr19);
    idt_table[20] = idt_isr(&isr20);
    idt_table[21] = idt_isr(&isr21);
    idt_table[22] = idt_isr(&isr22);
    idt_table[23] = idt_isr(&isr23);
    idt_table[24] = idt_isr(&isr24);
    idt_table[25] = idt_isr(&isr25);
    idt_table[26] = idt_isr(&isr26);
    idt_table[27] = idt_isr(&isr27);
    idt_table[28] = idt_isr(&isr28);
    idt_table[29] = idt_isr(&isr29);
    idt_table[30] = idt_isr(&isr30);
    idt_table[31] = idt_isr(&isr31);
    idt_table[32] = idt_isr(&isr32);
    idt_table[33] = idt_isr(&isr33);
    idt_table[34] = idt_isr(&isr34);
    idt_table[35] = idt_isr(&isr35);
    idt_table[36] = idt_isr(&isr36);
    idt_table[37] = idt_isr(&isr37);
    idt_table[38] = idt_isr(&isr38);
    idt_table[39] = idt_isr(&isr39);
    idt_table[40] = idt_isr(&isr40);
    idt_table[41] = idt_isr(&isr41);
    idt_table[42] = idt_isr(&isr42);
    idt_table[43] = idt_isr(&isr43);
    idt_table[44] = idt_isr(&isr44);
    idt_table[45] = idt_isr(&isr45);
    idt_table[46] = idt_isr(&isr46);
    idt_table[47] = idt_isr(&isr47);
    idt_table[48] = idt_isr(&isr48);
    idt_table[49] = idt_isr(&isr49);
    idt_table[50] = idt_isr(&isr50);
    idt_table[51] = idt_isr(&isr51);
    idt_table[52] = idt_isr(&isr52);
    idt_table[53] = idt_isr(&isr53);
    idt_table[54] = idt_isr(&isr54);
    idt_table[55] = idt_isr(&isr55);
    idt_table[56] = idt_isr(&isr56);
    idt_table[57] = idt_isr(&isr57);
    idt_table[58] = idt_isr(&isr58);
    idt_table[59] = idt_isr(&isr59);
    idt_table[60] = idt_isr(&isr60);
    idt_table[61] = idt_isr(&isr61);
    idt_table[62] = idt_isr(&isr62);
    idt_table[63] = idt_isr(&isr63);
    idt_table[64] = idt_isr(&isr64);
    idt_table[65] = idt_isr(&isr65);
    idt_table[66] = idt_isr(&isr66);
    idt_table[67] = idt_isr(&isr67);
    idt_table[68] = idt_isr(&isr68);
    idt_table[69] = idt_isr(&isr69);
    idt_table[70] = idt_isr(&isr70);
    idt_table[71] = idt_isr(&isr71);
    idt_table[72] = idt_isr(&isr72);
    idt_table[73] = idt_isr(&isr73);
    idt_table[74] = idt_isr(&isr74);
    idt_table[75] = idt_isr(&isr75);
    idt_table[76] = idt_isr(&isr76);
    idt_table[77] = idt_isr(&isr77);
    idt_table[78] = idt_isr(&isr78);
    idt_table[79] = idt_isr(&isr79);
    idt_table[80] = idt_isr(&isr80);
    idt_table[81] = idt_isr(&isr81);
    idt_table[82] = idt_isr(&isr82);
    idt_table[83] = idt_isr(&isr83);
    idt_table[84] = idt_isr(&isr84);
    idt_table[85] = idt_isr(&isr85);
    idt_table[86] = idt_isr(&isr86);
    idt_table[87] = idt_isr(&isr87);
    idt_table[88] = idt_isr(&isr88);
    idt_table[89] = idt_isr(&isr89);
    idt_table[90] = idt_isr(&isr90);
    idt_table[91] = idt_isr(&isr91);
    idt_table[92] = idt_isr(&isr92);
    idt_table[93] = idt_isr(&isr93);
    idt_table[94] = idt_isr(&isr94);
    idt_table[95] = idt_isr(&isr95);
    idt_table[96] = idt_isr(&isr96);
    idt_table[97] = idt_isr(&isr97);
    idt_table[98] = idt_isr(&isr98);
    idt_table[99] = idt_isr(&isr99);
    idt_table[100] = idt_isr(&isr100);
    idt_table[101] = idt_isr(&isr101);
    idt_table[102] = idt_isr(&isr102);
    idt_table[103] = idt_isr(&isr103);
    idt_table[104] = idt_isr(&isr104);
    idt_table[105] = idt_isr(&isr105);
    idt_table[106] = idt_isr(&isr106);
    idt_table[107] = idt_isr(&isr107);
    idt_table[108] = idt_isr(&isr108);
    idt_table[109] = idt_isr(&isr109);
    idt_table[110] = idt_isr(&isr110);
    idt_table[111] = idt_isr(&isr111);
    idt_table[112] = idt_isr(&isr112);
    idt_table[113] = idt_isr(&isr113);
    idt_table[114] = idt_isr(&isr114);
    idt_table[115] = idt_isr(&isr115);
    idt_table[116] = idt_isr(&isr116);
    idt_table[117] = idt_isr(&isr117);
    idt_table[118] = idt_isr(&isr118);
    idt_table[119] = idt_isr(&isr119);
    idt_table[120] = idt_isr(&isr120);
    idt_table[121] = idt_isr(&isr121);
    idt_table[122] = idt_isr(&isr122);
    idt_table[123] = idt_isr(&isr123);
    idt_table[124] = idt_isr(&isr124);
    idt_table[125] = idt_isr(&isr125);
    idt_table[126] = idt_isr(&isr126);
    idt_table[127] = idt_isr(&isr127);
    idt_table[128] = idt_isr(&isr128);
    idt_table[129] = idt_isr(&isr129);
    idt_table[130] = idt_isr(&isr130);
    idt_table[131] = idt_isr(&isr131);
    idt_table[132] = idt_isr(&isr132);
    idt_table[133] = idt_isr(&isr133);
    idt_table[134] = idt_isr(&isr134);
    idt_table[135] = idt_isr(&isr135);
    idt_table[136] = idt_isr(&isr136);
    idt_table[137] = idt_isr(&isr137);
    idt_table[138] = idt_isr(&isr138);
    idt_table[139] = idt_isr(&isr139);
    idt_table[140] = idt_isr(&isr140);
    idt_table[141] = idt_isr(&isr141);
    idt_table[142] = idt_isr(&isr142);
    idt_table[143] = idt_isr(&isr143);
    idt_table[144] = idt_isr(&isr144);
    idt_table[145] = idt_isr(&isr145);
    idt_table[146] = idt_isr(&isr146);
    idt_table[147] = idt_isr(&isr147);
    idt_table[148] = idt_isr(&isr148);
    idt_table[149] = idt_isr(&isr149);
    idt_table[150] = idt_isr(&isr150);
    idt_table[151] = idt_isr(&isr151);
    idt_table[152] = idt_isr(&isr152);
    idt_table[153] = idt_isr(&isr153);
    idt_table[154] = idt_isr(&isr154);
    idt_table[155] = idt_isr(&isr155);
    idt_table[156] = idt_isr(&isr156);
    idt_table[157] = idt_isr(&isr157);
    idt_table[158] = idt_isr(&isr158);
    idt_table[159] = idt_isr(&isr159);
    idt_table[160] = idt_isr(&isr160);
    idt_table[161] = idt_isr(&isr161);
    idt_table[162] = idt_isr(&isr162);
    idt_table[163] = idt_isr(&isr163);
    idt_table[164] = idt_isr(&isr164);
    idt_table[165] = idt_isr(&isr165);
    idt_table[166] = idt_isr(&isr166);
    idt_table[167] = idt_isr(&isr167);
    idt_table[168] = idt_isr(&isr168);
    idt_table[169] = idt_isr(&isr169);
    idt_table[170] = idt_isr(&isr170);
    idt_table[171] = idt_isr(&isr171);
    idt_table[172] = idt_isr(&isr172);
    idt_table[173] = idt_isr(&isr173);
    idt_table[174] = idt_isr(&isr174);
    idt_table[175] = idt_isr(&isr175);
    idt_table[176] = idt_isr(&isr176);
    idt_table[177] = idt_isr(&isr177);
    idt_table[178] = idt_isr(&isr178);
    idt_table[179] = idt_isr(&isr179);
    idt_table[180] = idt_isr(&isr180);
    idt_table[181] = idt_isr(&isr181);
    idt_table[182] = idt_isr(&isr182);
    idt_table[183] = idt_isr(&isr183);
    idt_table[184] = idt_isr(&isr184);
    idt_table[185] = idt_isr(&isr185);
    idt_table[186] = idt_isr(&isr186);
    idt_table[187] = idt_isr(&isr187);
    idt_table[188] = idt_isr(&isr188);
    idt_table[189] = idt_isr(&isr189);
    idt_table[190] = idt_isr(&isr190);
    idt_table[191] = idt_isr(&isr191);
    idt_table[192] = idt_isr(&isr192);
    idt_table[193] = idt_isr(&isr193);
    idt_table[194] = idt_isr(&isr194);
    idt_table[195] = idt_isr(&isr195);
    idt_table[196] = idt_isr(&isr196);
    idt_table[197] = idt_isr(&isr197);
    idt_table[198] = idt_isr(&isr198);
    idt_table[199] = idt_isr(&isr199);
    idt_table[200] = idt_isr(&isr200);
    idt_table[201] = idt_isr(&isr201);
    idt_table[202] = idt_isr(&isr202);
    idt_table[203] = idt_isr(&isr203);
    idt_table[204] = idt_isr(&isr204);
    idt_table[205] = idt_isr(&isr205);
    idt_table[206] = idt_isr(&isr206);
    idt_table[207] = idt_isr(&isr207);
    idt_table[208] = idt_isr(&isr208);
    idt_table[209] = idt_isr(&isr209);
    idt_table[210] = idt_isr(&isr210);
    idt_table[211] = idt_isr(&isr211);
    idt_table[212] = idt_isr(&isr212);
    idt_table[213] = idt_isr(&isr213);
    idt_table[214] = idt_isr(&isr214);
    idt_table[215] = idt_isr(&isr215);
    idt_table[216] = idt_isr(&isr216);
    idt_table[217] = idt_isr(&isr217);
    idt_table[218] = idt_isr(&isr218);
    idt_table[219] = idt_isr(&isr219);
    idt_table[220] = idt_isr(&isr220);
    idt_table[221] = idt_isr(&isr221);
    idt_table[222] = idt_isr(&isr222);
    idt_table[223] = idt_isr(&isr223);
    idt_table[224] = idt_isr(&isr224);
    idt_table[225] = idt_isr(&isr225);
    idt_table[226] = idt_isr(&isr226);
    idt_table[227] = idt_isr(&isr227);
    idt_table[228] = idt_isr(&isr228);
    idt_table[229] = idt_isr(&isr229);
    idt_table[230] = idt_isr(&isr230);
    idt_table[231] = idt_isr(&isr231);
    idt_table[232] = idt_isr(&isr232);
    idt_table[233] = idt_isr(&isr233);
    idt_table[234] = idt_isr(&isr234);
    idt_table[235] = idt_isr(&isr235);
    idt_table[236] = idt_isr(&isr236);
    idt_table[237] = idt_isr(&isr237);
    idt_table[238] = idt_isr(&isr238);
    idt_table[239] = idt_isr(&isr239);
    idt_table[240] = idt_isr(&isr240);
    idt_table[241] = idt_isr(&isr241);
    idt_table[242] = idt_isr(&isr242);
    idt_table[243] = idt_isr(&isr243);
    idt_table[244] = idt_isr(&isr244);
    idt_table[245] = idt_isr(&isr245);
    idt_table[246] = idt_isr(&isr246);
    idt_table[247] = idt_isr(&isr247);
    idt_table[248] = idt_isr(&isr248);
    idt_table[249] = idt_isr(&isr249);
    idt_table[250] = idt_isr(&isr250);
    idt_table[251] = idt_isr(&isr251);
    idt_table[252] = idt_isr(&isr252);
    idt_table[253] = idt_isr(&isr253);
    idt_table[254] = idt_isr(&isr254);
    idt_table[255] = idt_isr(&isr255);


    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr)
                     :);
    __asm__ volatile("sti");
}

void _panic() {
    printf("Kernel panic!\n");
    while (1) {
        asm("hlt");
    }
}



void (*irq_handlers[16])(void) = {};

void __attribute__((__cdecl)) isr_handler(registers_t regs)
{
    if (regs.int_no < 32) {
        printf("Interrupt %i. Error code %x\n", regs.int_no, regs.err_code);
        _panic();
    } else if (regs.int_no >= 32 && regs.int_no <= 40) {
        uint32_t pic_irq = regs.int_no - 32;
        if (irq_handlers[pic_irq] != 0) {
            irq_handlers[pic_irq]();
        } else {
            printf("Interrupt %i (PIC IRQ %i). Error code %x\n", regs.int_no, pic_irq, regs.err_code);
        };

        PIC_sendEOI(pic_irq);
    }  
}

void irq_sethandler(int irq, void (*handler)(void)) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_unsethandler(int irq) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = 0;
    }
}
