program test_CEILING

   

   implicit none

   
   REAL :: in0 = ( 0.42 )
   
 
   REAL :: o_host, o_device 

    o_host = CEILING( in0)

    !$OMP target map(from:o_device)
    o_device = CEILING( in0)
    !$OMP END TARGET

    
    IF ( ABS(o_host-o_device) > EPSILON(  o_host   ) ) THEN
    
        write(*,*)  'Expected ', o_host, ' Got ', o_device
        call exit(1)
    ENDIF

end program test_CEILING
