! kompajliranje: ifort -standard-semantics -c mymod.f90

module mymod

  use, intrinsic :: iso_c_binding

  type, bind(c) :: tocka
     real(c_double) :: x, y, z
  end type tocka

contains

  ! Fortran startna funkcija dretve.
  ! Navedemo kako zelimo da se zove iz C-a.
  function dretva(arg) bind(c,name='dretva')
    use, intrinsic :: iso_c_binding

    type(c_ptr) :: dretva
    type(c_ptr), value :: arg

    type(tocka), pointer :: t
    real(c_double), pointer :: mag

    real(c_double), intrinsic :: sqrt

    call c_f_pointer(arg, t)

    allocate(mag)
    mag = sqrt(t%x * t%x + t%y * t%y + t%z * t%z)

    ! vratimo rezultat
    dretva = c_loc(mag)
  end function dretva

end module mymod
