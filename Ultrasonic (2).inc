; Configure GPIO for the Ultrasonic sensor pins
; ---------------------------------------------------------
UltrasonicInit:

	  sbi	    DDRB, DDB1			  ; set trigger to output
	  cbi	    PORTB, PORTB1		  ; set trigger to low

	  cbi	    DDRB, DDB2			  ; set echo to input
	  cbi	    PORTB, PORTB2		  ; high imp

	  ret

; initialize the ultrasonic sensor
; return to the loop
; ---------------------------------------------------------
Measure_Cycle:
	  clr	    r0				  ; inch counter
	  call	    Trigger

Start_while:					  ; wait for echo pin to detect signal
	  sbis	    PINB, PINB2
	  rjmp	    Start_while			  ; if detect signal, start cycle
	    
Echo_Cycle:					  ;
	  sbis	    PINB, PINB2			  ; if echo detects signal
	  rjmp	    RetMeasure			  ; else return inch	
						  ;	
	  call	    DelayMeasure		  ; delay for every inch	
	  inc	    r0				  ; ++ inch counter
						  ;		
	  rjmp	    Echo_Cycle			  ; back to start

RetMeasure:
	  ret					  ; R0


; We need a 10us delay for the trigger to get a proper echo read 
	    
; 10us = (1/16MHz) / 5cycles * 32
; ---------------------------------------------------------
Trigger:
	  sbi	    PORTB, PORTB1		  ; set trigger pin high

	  ldi	    r16, 32			  ; 32 * 5
TriggerDelay:
	  nop					  ; 1 cycle
	  nop					  ; 1 cycle
	  dec	    r16				  ; 1 cycle
	  brne	    TriggerDelay		  ; 2 cycles

	  cbi	    PORTB, PORTB1		  ; clear trigger pin low

	  ret     

; Measurement timer 148us/inch
; ---------------------------------------------------------
DelayMeasure:
	  clr	    r20
	  sts	    TCNT2, r20			  ; clear counter

	  ldi	    r20, 36
	  sts	    OCR2A, r20			  ; 148us @ clk/64

	  ldi	    r20, (1<<WGM21)
	  sts	    TCCR2A, r20			  ; ctc mode

	  ldi	    r20, (1<<CS22)
	  sts	    TCCR2B, r20			  ; clk/64

DelayMeasureWait:
	  sbis	    TIFR2,OCF2A			  ; wait for match
	  rjmp	    DelayMeasureWait
     
	  sbi	    TIFR2,OCF2A			  ; reset flag

	  clr	    r20
	  sts	    TCCR2B, r20			  ; stop timer

	  ret
     