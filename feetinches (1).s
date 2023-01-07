#define __SFR_OFFSET 0
#include <avr/io.h>

#include "vectortable.inc"
#include "Ultrasonic.inc"


          
; Set up the PINs we are using for the LEDs
; -----------------------------------------------------------------------------
          sbi       DDRD, DDD3                    ; Pin D3 to input
          cbi       DDRB, DDB0                    ; Pin D4 to output
          cbi       DDRD, DDD5                    ; Pin D5 to output
          cbi       DDRD, DDD6                    ; Pin D6 to output
          cbi       DDRD, DDD7                    ; Pin D7 to output
          
          cbi       PORTD, PORTD3                   ; high imp or pull up
          sbi       PORTD, PORTD4                 ; Set motor pin to output
          sbi       PORTD, PORTD5                 ; Set green LED pin to output 
          sbi       PORTD, PORTD6                 ; Set yellow LED pin to output
          sbi       PORTD, PORTD7                 ; Set red LED pin to output

          

; Main routine performs initialization and then 
; continually loops through application code
; ---------------------------------------------------------
main:
          ; initialize the ultrasonic sensor
          call      UltrasonicInit
          
          
          ; config Pin Change Int for LED
	ldi	r21, (1<<PCIE2)		; Pin Change Port-B
	sts	PCICR, r21

	ldi	r21, (1<<PCINT19)		; Pin Change on PD3
	sts	PCMSK2, r21			

          
          ; turn on global interrupts
          sei
    
loop:                                             ; [
          
          cli                                     ; disable interrupts

          call      Measure_Cycle                ; start a measurement (R0 = inches)

          call      START_IF
          
	sei                                     ; enable interrupts
     
          call      Delay                         ; wait 1s before taking next measurement

          rjmp      loop                          ; ]

          
; Interupt 
; ---------------------------------------------------------
LED_isr:
          sbis	PIND, PIND3	; if button pressed (cleared low 0)
          cbi       PORTB, PORTB0
          reti
; turns off LED 
; ---------------------------------------------------------
        
; ----- LED LOGIC ---------------------------------
          
START_IF: 
      
        
          ldi       r29, 24             ;r29, 2ft 
          ldi       r30, 48             ; r30, 4ft
          
          mov       r18, r0             ; Ones place (In Feet)
          
          cp        r0, r29             ; if (feet read in) - 3 < 0
          brlo      Close_LED_ON        ; branch if true
     
          cp        r0, r30             ; if (feet read in) - 6 < 0
          brlo      Middle_LED_ON       ; branch if true
     
          rjmp      Far_LED_ON          ; else -> greater than 6 feet away         
     
; -----------------------------------------------------------------------------
     
; bracnh 1    
; < 3 feet -------> D7
Close_LED_ON:         
          cbi       PORTD, PORTD6                 ; Turn off all other LEDs (maybe a make a func?)
          cbi       PORTD, PORTD5
                                 
          sbi       PORTD, PORTD7                 ; Turn on Red LED (D7)

                                                  ; Turn on yellow
          sbi       PORTB, PORTB0                    
          
          
          rjmp      End_IF                        ; jump to end if


; < 6 feet -----> D6
Middle_LED_ON:
          cbi       PORTD, PORTD5                 ; Turn off all other LEDs
          cbi       PORTD, PORTD7
 
          
          sbi       PORTD, PORTD6                 ; Turn on Yellow LED (D6)
          
      
          rjmp      End_IF                        ; jump to end if
          
; else ---> D5
Far_LED_ON:
          cbi       PORTD, PORTD6                 ; Turn off all other LEDs
          cbi       PORTD, PORTD7 
                                                  
          sbi       PORTD, PORTD5                 ; Turn on Green LED (D5)
                                                  
                                                  ; (We step into the END so no jump needed!)
          
End_IF:       
ret                                               
     

; sleep for 1s between measurement cycles 
; ---------------------------------------------------------
Delay:
     
     ; cannot use io commands because they are "out of range"
     
     clr  r20                 ; clear counter
     sts  TCNT1H, r20         
     sts  TCNT1L, r20

     ldi  r20, 0x3D           ; match at 15624 aka 100000 microseconds or 1 second
     sts  OCR1AH, r20
     ldi  r20, 0x08
     sts  OCR1AL, r20

     clr  r20
     sts  TCCR1A, r20         ; clearing for ctc

     ldi  r20, 0b00001101
     sts  TCCR1B, r20         ; ctc, clk/1024

Delay_wait:
          
     sbis TIFR1, OCF1A        ; wait for flag
     rjmp Delay_wait

     clr  r20
     sts  TCCR1B, r20         ; 

     sbi  TIFR1, OCF1A        ; clear flag

     ret