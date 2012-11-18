! kompajliranje: ifort -standard-semantics demo.f90 forthr.o mymod.o forthr_c.o

program demo

  use forthr
  use mymod

  use, intrinsic :: iso_c_binding

  ! ako uzimamo adresu varijable sa c_loc, varijabla mora biti target
  type(tocka), target :: t
  type(c_funptr) :: fn
  type(c_ptr) :: arg, ret
  integer(c_intptr_t) :: pth
  real(c_double), pointer :: mag

  print *, 'unesi koordinatu x'
  read *, t%x
  print *, 'unesi koordinatu y'
  read *, t%y
  print *, 'unesi koordinatu z'
  read *, t%z

  ! uzmi adresu startne funkcije za dretvu
  fn = c_funloc(dretva)
  ! uzmi adresu strukture koju saljes dretvi
  arg = c_loc(t)
  print *, 'create_thread: ', create_thread(fn, arg, pth)
  print *, 'join_thread: ', join_thread(pth, ret)
  ! pretvori povratni C pointer u Fortran pointer
  call c_f_pointer(ret, mag)
  print *, 'korijen iz sume kvadrata koordinata:', mag
  ! oslobodi memoriju alociranu u dretvi
  deallocate(mag)

end program demo
