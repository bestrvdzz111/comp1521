# MIPS assembler to compute Fibonacci numbers

   .data
msg1:
   .asciiz "n = "
msg2:
   .asciiz "fib(n) = "
msg3:
   .asciiz "n must be > 0\n" 
eol:
   .asciiz "\n"

   .text


# int main(void)
# {
#    int n;
#    printf("n = ");
#    scanf("%d", &n);
#    if (n >= 1)
#       printf("fib(n) = %d\n", fib(n));
#    else {
#       printf("n must be > 0\n");
#       exit(1);
#    }
#    return 0;
# }

   .globl main
main:
   # prologue
   addi $sp, $sp, -4
   sw   $fp, ($sp)
   move $fp, $sp
   addi $sp, $sp, -4
   sw   $ra, ($sp)

   # function body
   la   $a0, msg1       #printf("n = ");
   li   $v0, 4
   syscall

   li   $v0, 5          #scanf("%d", &n);
   syscall
   move $a0, $v0

   # ... add code to check (n >= 1)
   # ... print an error message, if needed
   # ... and return a suitable value from main()

   li $t0,0
   bge $a0,$t0,else
   
   la $a0,msg3
   li $v0,4
   syscall
   j exit

else:
   jal  fib             # $s0 = fib(n);
   nop
   move $s0, $v0

   la   $a0, msg2       # printf((fib(n) = ");
   li   $v0, 4
   syscall

   move $a0, $s0        # printf("%d", $s0);
   li   $v0, 1
   syscall

   la   $a0, eol       # printf("\n");
   li   $v0, 4
   syscall

   li $v0,1

exit:
   # epilogue
   lw   $ra, ($sp)
   addi $sp, $sp, 4
   lw   $fp, ($sp)
   addi $sp, $sp, 4
   jr   $ra


# int fib(int n)
# {
#    if (n < 1)
#       return 0;
#    else if (n == 1)
#       return 1;
#    else
#       return fib(n-1) + fib(n-2);
# }

fib:
   addi $sp,$sp,-12          #  to storing the return adress on stack
   sw $ra,0($sp)
   sw $s0,4($sp)
   sw $s1,8($sp)

   # function body
   
   move $s0,$a0

   beq $s0,1,zero
   blt $s0,1,done
   
   addi $a0,$s0,-1  # (n-1)
   jal fib
   move $s1,$v0   # s1 will store all the v0 value return back fib(n-1)

   addi $a0,$s0,-2
   jal fib

   add $v0,$v0,$s1    # v0 = fib(n-2)

  #epilogue

exitfib:
   lw $ra,0($sp)
   lw $s0,4($sp)
   lw $s1,8($sp)
   addi $sp,$sp,12
   jr $ra

done:
   li $v0,0
   j exitfib
zero:
   li $v0,1
   j exitfib

   jr   $ra
