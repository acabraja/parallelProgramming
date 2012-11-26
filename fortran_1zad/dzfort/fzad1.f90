program fzad1

  use finterface
  use mymodul

  use, intrinsic :: iso_c_binding

  ! ako uzimamo adresu varijable sa c_loc, varijabla mora biti target
  type(podatak), target :: dat
  type(c_funptr) :: fn
  type(c_ptr) :: arg, ret ! argument funkcije dretva i povratna vrijednost nakon join_thread resoektivno
  integer(c_intptr_t), allocatable :: pth(:)
  integer(c_int), pointer :: povratna_vrijednost
  integer :: n ! dimenzija matrice nalati se u datoteci sa matricom A kao i broj dretvi  
  integer :: p ! broj dretvi
  integer :: ostatak, koliko_poslano, sigurno_saljem ! pomocne varijable za raspodjelu po dretvama 
  
  !citanje iz datoteke
  
  open (unit=99, file='MatricaA.txt', status='old', action='read')
  read(99, *), n
  read(99, *), p
  allocate(A(n*n))
  read(99,*) A
  !write(*,*) A
  
  open ( unit=99, file='MatricaB.txt', status='old', action='read')
  allocate(B(n*n))
  read(99, *) B
  !write(*,*) B
  
  ! alokacija broja dretvi i matrice rezultata
  
  allocate(C(n*n,n*n))
  allocate(pth(p))
  fn = c_funloc(dretva)

  ostatak = mod(n*n,p)
  koliko_poslano = 0
  sigurno_saljem = (n*n)/p
  
  
  
  ! raspodjela posla
  do i=1,p
    
    if ( ostatak .gt. 0) then
       ostatak = ostatak - 1
       dat%pocetak = 1 + koliko_poslano
       dat%kraj = dat%pocetak + sigurno_saljem 
       koliko_poslano = dat%kraj
       dat%duljina = n
       print *, 'dretva ',i,' pocetka = ',dat%pocetak,', kraj = ',dat%kraj
    else
      ostatak = ostatak - 1
      dat%pocetak = 1 + koliko_poslano
      dat%kraj = dat%pocetak + sigurno_saljem - 1
      koliko_poslano = dat%kraj
      dat%duljina = n
      print *, 'dretva ',i,' pocetka = ',dat%pocetak,', kraj = ',dat%kraj
    endif
    
    arg = c_loc(dat)
    print *, 'create_thread: ', create_thread(fn, arg, pth(i))
       
  enddo
  
  
  ! join svih dretvi
  
  do i=1,p
    print *, 'join_thread: ', join_thread(pth(i), ret)
  enddo
  
  open ( unit=99, file='MatricaC.txt', status='old', action='write')
  do i=1,n*n
    do j=1,n*n
      write(99,'(c4)') C(i,j)
    enddo
  enddo

end program fzad1
