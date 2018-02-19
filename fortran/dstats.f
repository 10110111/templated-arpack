c
c\SCCS Information: @(#) 
c FILE: stats.F   SID: 2.1   DATE OF SID: 4/19/96   RELEASE: 2
c     %---------------------------------------------%
c     | Initialize statistic and timing information |
c     | for symmetric Arnoldi code.                 |
c     %---------------------------------------------%
 
c      subroutine dstats
c
cc     %--------------------------------%
cc     | See stat.doc for documentation |
cc     %--------------------------------%
c      include   'stat.h'
c 
cc     %-----------------------%
cc     | Executable Statements |
cc     %-----------------------%
c
c      nopx   = 0
c      nbx    = 0
c      nrorth = 0
c      nitref = 0
c      nrstrt = 0
c 
c      tsaupd = 0.0D+0
c      tsaup2 = 0.0D+0
c      tsaitr = 0.0D+0
c      tseigt = 0.0D+0
c      tsgets = 0.0D+0
c      tsapps = 0.0D+0
c      tsconv = 0.0D+0
c      titref = 0.0D+0
c      tgetv0 = 0.0D+0
c      trvec  = 0.0D+0
c 
cc     %----------------------------------------------------%
cc     | User time including reverse communication overhead |
cc     %----------------------------------------------------%
c      tmvopx = 0.0D+0
c      tmvbx  = 0.0D+0
c 
c      return
cc
cc     End of dstats
cc
c      end
