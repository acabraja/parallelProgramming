
program test
  
!  real, dimension(5) :: A,B
!  real sdot
!  external sdot
!  A = (/1,2,3,4,5/)
!  B = (/1,1,1,1,1/)
!  
!  !call sdot(5,A,1,B,1)
!  
!  write(*,*) sdot(5,A,1,B,1)     
  real, dimension(:), allocatable :: x
      integer :: n
 
      open (unit=99, file='array.txt', status='old', action='read')
      read(99, *), n
      allocate(x(n*n))
      read(99,*) x
      !write (*,*) x()
      write(*,*) x
end program test
