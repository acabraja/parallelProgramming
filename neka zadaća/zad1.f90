subroutine pokusaj
  print *,"Usao"
end subroutine



program main 
  use f_pthread
  implicit none

  integer, parameter :: N=2
  integer i,j
  common/data/i

  complex*16, parameter :: imag1 = cmplx(0.d0, 1.d0)
  complex*16 :: a(N,N), x(N), y(N)

  complex*16 :: alpha, beta

  a(:,:)=imag1;
  x(:)=1.d0
  y(:)=0.d0

  alpha=1.d0; beta=0.d0

  call zgemv('N',N,N,alpha,a,N,x,1,beta,y,1)
  
  j=1
  
  !create_pth(pokusaj,j)
  
  print *, y

  i=1
  call test()

end program main
