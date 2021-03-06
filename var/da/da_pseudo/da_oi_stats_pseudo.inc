subroutine da_oi_stats_pseudo (stats_unit, iv)

   !-----------------------------------------------------------------------
   ! Purpose: TBD
   !-----------------------------------------------------------------------

   implicit none

   integer,        intent (in)      :: stats_unit    ! Output unit for stats.
   type (iv_type), intent (in)      :: iv            ! O-B

   type (stats_pseudo_type)         :: stats
   integer                          :: nu, nv, nt, np, nq
   integer                          :: n

   if (trace_use_dull) call da_trace_entry("da_oi_stats_pseudo")

   nu = 0
   nv = 0
   nt = 0
   np = 0
   nq = 0
   
   stats%maximum%u = maxmin_type(missing_r, 0, 0)
   stats%maximum%v = maxmin_type(missing_r, 0, 0)
   stats%maximum%t = maxmin_type(missing_r, 0, 0)
   stats%maximum%p = maxmin_type(missing_r, 0, 0)
   stats%maximum%q = maxmin_type(missing_r, 0, 0)
   stats%minimum%u = maxmin_type(-missing_r, 0, 0)
   stats%minimum%v = maxmin_type(-missing_r, 0, 0)
   stats%minimum%t = maxmin_type(-missing_r, 0, 0)
   stats%minimum%p = maxmin_type(-missing_r, 0, 0)
   stats%minimum%q = maxmin_type(-missing_r, 0, 0)
   stats%average = residual_pseudo1_type(0.0, 0.0, 0.0, 0.0, 0.0)
   stats%rms_err = stats%average

   do n=1, iv%info(pseudo)%nlocal
      if (iv%info(pseudo)%proc_domain(1,n)) then
         call da_stats_calculate(n, 0, iv%pseudo(n)%u%qc, & 
            iv%pseudo(n)%u%inv, nu, & 
            stats%minimum%u, stats%maximum%u, &
            stats%average%u, stats%rms_err%u)
         call da_stats_calculate(n, 0, iv%pseudo(n)%v%qc, & 
            iv%pseudo(n)%v%inv, nv, & 
            stats%minimum%v, stats%maximum%v, &
            stats%average%v, stats%rms_err%v)
         call da_stats_calculate(n, 0, iv%pseudo(n)%t%qc, & 
            iv%pseudo(n)%t%inv, nt, & 
            stats%minimum%t, stats%maximum%t, &
            stats%average%t, stats%rms_err%t)
         call da_stats_calculate(n, 0, iv%pseudo(n)%p%qc, & 
            iv%pseudo(n)%p%inv, np, & 
            stats%minimum%p, stats%maximum%p, &
            stats%average%p, stats%rms_err%p)
         call da_stats_calculate(n, 0, iv%pseudo(n)%q%qc, & 
            iv%pseudo(n)%q%inv, nq, & 
            stats%minimum%q, stats%maximum%q, &
            stats%average%q, stats%rms_err%q)
      end if    ! end if (iv%info(pseudo)%proc_domain(1,n))
   end do    

   ! Do inter-processor communication to gather statistics.
   call da_proc_sum_int(nu)
   call da_proc_sum_int(nv)
   call da_proc_sum_int(nt)
   call da_proc_sum_int(np)
   call da_proc_sum_int(nq)
   
   call da_proc_stats_combine(stats%average%u, stats%rms_err%u, &
      stats%minimum%u%value, stats%maximum%u%value, &
      stats%minimum%u%n, stats%maximum%u%n, &
      stats%minimum%u%l, stats%maximum%u%l)
   call da_proc_stats_combine(stats%average%v, stats%rms_err%v, &
      stats%minimum%v%value, stats%maximum%v%value, &
      stats%minimum%v%n, stats%maximum%v%n, &
      stats%minimum%v%l, stats%maximum%v%l)
   call da_proc_stats_combine(stats%average%t, stats%rms_err%t, &
      stats%minimum%t%value, stats%maximum%t%value, &
      stats%minimum%t%n, stats%maximum%t%n, &
      stats%minimum%t%l, stats%maximum%t%l)
   call da_proc_stats_combine(stats%average%p, stats%rms_err%p, &
      stats%minimum%p%value, stats%maximum%p%value, &
      stats%minimum%p%n, stats%maximum%p%n, &
      stats%minimum%p%l, stats%maximum%p%l)
   call da_proc_stats_combine(stats%average%q, stats%rms_err%q, &
      stats%minimum%q%value, stats%maximum%q%value, &
      stats%minimum%q%n, stats%maximum%q%n, &
      stats%minimum%q%l, stats%maximum%q%l)
   
   if (rootproc) then
      if (nu /= 0 .or. nv /= 0 .OR. nt /= 0 .or. np /= 0 .OR. nq /= 0) then
         write(unit=stats_unit, fmt='(/a/)') ' O-B Diagnostics for pseudo'
         call da_print_stats_pseudo(stats_unit, nu, nv, nt, np, nq, stats)
      end if
   end if

   if (trace_use_dull) call da_trace_exit("da_oi_stats_pseudo")

end subroutine da_oi_stats_pseudo


