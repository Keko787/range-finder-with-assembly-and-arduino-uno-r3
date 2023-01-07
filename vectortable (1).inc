; vector table for interrupts
;----------------------------------------------------------
BEGIN_VECTORS:                ; Reset
     jmp  main

INT0addr:                     ; External Interrupt Request 0
     nop       
     nop
INT1addr:                     ; External Interrupt Request 1
     nop
     nop
PCI0addr:                     ; Pin Change Interrupt Request 0 
    nop       
    nop
PCI1addr:                     ; Pin Change Interrupt Request 1
     nop       
     nop
PCI2addr:                     ; Pin Change Interrupt Request 2
     jmp  LED_isr
     ;nop
     ;nop
WDTaddr:                      ; Watchdog Time-out Interrupt
     nop       
     nop
OC2Aaddr:                     ; Timer/Counter2 Compare Match A
     nop       
     nop
OC2Baddr:                     ; Timer/Counter2 Compare Match A
     nop       
     nop
OVF2addr:                     ; Timer/Counter2 Overflow
     nop       
     nop
ICP1addr:                     ; Timer/Counter1 Capture Event
     nop       
     nop
OC1Aaddr:                     ; Timer/Counter1 Compare Match A
     nop       
     nop
OC1Baddr:                     ; Timer/Counter1 Compare Match B
     nop       
     nop
OVF1addr:                     ; Timer/Counter1 Overflow
     nop       
     nop
OC0Aaddr:                     ; TimerCounter0 Compare Match A
    nop
    nop
    ;jmp	DisplayMux_isr       

OC0Baddr:                     ; TimerCounter0 Compare Match B
     nop       
     nop
OVF0addr:                     ; Timer/Couner0 Overflow
     nop       
     nop
SPIaddr:                      ; SPI Serial Transfer Complete
     nop       
     nop
URXCaddr:                     ; USART Rx Complete
     nop       
     nop
UDREaddr:                     ; USART, Data Register Empty
     nop       
     nop
UTXCaddr:                     ; USART Tx Complete
     nop       
     nop
ADCCaddr:                     ; ADC Conversion Complete
     nop       
     nop
ERDYaddr:                     ; EEPROM Ready
     nop       
     nop
ACIaddr:                      ; Analog Comparator
     nop       
     nop
TWIaddr:                      ; Two-wire Serial Interface
     nop       
     nop
SPMRaddr:                     ; Store Program Memory Read
     nop       
     nop
END_VECTORS:



