
# Multiboot header: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


# Setup stack pointer(ESP) - 16KiB
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384
stack_top:

# Our kernel starts at _start
.section .text
.global _start
.type _start, @function
_start:
        # ESP <- top of stack
	movl $stack_top, %esp

	call kernel_main # In kernel.c

	cli
	hlt
.Lhang:
	jmp .Lhang

# size of _start to current location `.` minus the start of the function
.size _start, . - _start
