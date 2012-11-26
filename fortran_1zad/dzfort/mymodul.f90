! kompajliranje: ifort -standard-semantics -c mymod.f90

module mymodul

  use, intrinsic :: iso_c_binding
  double complex, allocatable :: A(:),B(:),C(:,:)

  type, bind(c) :: podatak
     integer(c_int) :: pocetak, kraj, duljina
  end type podatak

contains

  ! Fortran startna funkcija dretve.
  ! Navedemo kako zelimo da se zove iz C-a.
  function dretva(arg) bind(c,name='dretva')
    use, intrinsic :: iso_c_binding

    type(c_ptr) :: dretva
    type(c_ptr), value :: arg
    type(podatak), pointer :: dat
    real(c_double), pointer :: mag
    double complex,allocatable :: pom(:)
    external zscal
    call c_f_pointer(arg, dat)
    
    allocate(pom(dat%duljina*dat%duljina))
    
    do i=dat%pocetak,dat%kraj
      do j=1,dat%duljina*dat%duljina
        pom(j) = B(j)
      enddo
        
      call zscal(dat%duljina*dat%duljina,A(i),pom,1)
      
      do l=1,dat%duljina
        do k=1, dat%duljina
          C(mod(i-1,dat%duljina)*dat%duljina+k,((i-1)/dat%duljina)*dat%duljina+l)=pom((l-1)*dat%duljina+k)
        enddo
      enddo
    enddo  
    
    deallocate(pom)
    ! vratimo rezultat
    allocate(mag)
    mag = dat%pocetak + dat%kraj
    dretva = c_loc(mag)
  end function dretva

end module mymodul


