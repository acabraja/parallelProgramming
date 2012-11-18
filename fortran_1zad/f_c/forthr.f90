! kompajliranje: ifort -standard-semantics -c forthr.f90

module forthr

  use, intrinsic :: iso_c_binding

  interface
     function create_thread(fn, arg, pth) bind(c,name='create_thread')
       use, intrinsic :: iso_c_binding
       integer(c_int) :: create_thread
       type(c_funptr), value :: fn
       type(c_ptr), value :: arg
       integer(c_intptr_t), intent(out) :: pth
     end function create_thread
  end interface

  interface
     function join_thread(pth, ret) bind(c,name='join_thread')
       use, intrinsic :: iso_c_binding
       integer(c_int) :: join_thread
       integer(c_intptr_t), value :: pth
       type(c_ptr), intent(out) :: ret
     end function join_thread
  end interface

end module forthr
