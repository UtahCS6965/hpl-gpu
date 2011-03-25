/*
 *  -- High Performance Computing Linpack Benchmark (HPL-GPU)
 *     HPL-GPU - 1.1 - 2011
 *
 *     David Rohr
 *     Matthias Kretz
 *     Matthias Bach
 *     Goethe Universität, Frankfurt am Main
 *     Frankfurt Institute for Advanced Studies
 *     (C) Copyright 2010 All Rights Reserved
 *
 *     Antoine P. Petitet
 *     University of Tennessee, Knoxville
 *     Innovative Computing Laboratory
 *     (C) Copyright 2000-2008 All Rights Reserved
 *
 *  -- Copyright notice and Licensing terms:
 *
 *  Redistribution  and  use in  source and binary forms, with or without
 *  modification, are  permitted provided  that the following  conditions
 *  are met:
 *
 *  1. Redistributions  of  source  code  must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce  the above copyright
 *  notice, this list of conditions,  and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *
 *  3. All  advertising  materials  mentioning  features  or  use of this
 *  software must display the following acknowledgements:
 *  This  product  includes  software  developed  at  the  University  of
 *  Tennessee, Knoxville, Innovative Computing Laboratory.
 *  This product  includes software  developed at the Frankfurt Institute
 *  for Advanced Studies.
 *
 *  4. The name of the  University,  the name of the  Laboratory,  or the
 *  names  of  its  contributors  may  not  be used to endorse or promote
 *  products  derived   from   this  software  without  specific  written
 *  permission.
 *
 *  -- Disclaimer:
 *
 *  THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 *  OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
 *  SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ======================================================================
 */

/*
 * Include files
 */
#include "hpl.h"

#include "util_timer.h"
#include "util_trace.h"
/*
 * Do not use  MPI user-defined data types no matter what.  This routine
 * is used for small contiguous messages.
 */
#ifdef HPL_USE_MPI_DATATYPE
#undef HPL_USE_MPI_DATATYPE
#endif

int HPL_send
(
   double *                         SBUF,
   int                              SCOUNT,
   int                              DEST,
   int                              STAG,
   MPI_Comm                         COMM
)
{
/* 
 * Purpose
 * =======
 *
 * HPL_send is a simple wrapper around  MPI_Send.  Its  main  purpose is
 * to  allow for some  experimentation / tuning  of this simple routine.
 * Successful  completion  is  indicated  by  the  returned  error  code
 * MPI_SUCCESS.  In the case of messages of length less than or equal to
 * zero, this function returns immediately.
 *
 * Arguments
 * =========
 *
 * SBUF    (local input)                 double *
 *         On entry, SBUF specifies the starting address of buffer to be
 *         sent.
 *
 * SCOUNT  (local input)                 int
 *         On entry,  SCOUNT  specifies  the number of  double precision
 *         entries in SBUF. SCOUNT must be at least zero.
 *
 * DEST    (local input)                 int
 *         On entry, DEST specifies the rank of the receiving process in
 *         the communication space defined by COMM.
 *
 * STAG    (local input)                 int
 *         On entry,  STAG specifies the message tag to be used for this
 *         communication operation.
 *
 * COMM    (local input)                 MPI_Comm
 *         The MPI communicator identifying the communication space.
 *
 * ---------------------------------------------------------------------
 */ 
START_TRACE( SEND )

/*
 * .. Local Variables ..
 */
#ifdef HPL_USE_MPI_DATATYPE
   MPI_Datatype               type;
#endif
   int                        ierr;
/* ..
 * .. Executable Statements ..
 */
   if( SCOUNT <= 0 ) return( HPL_SUCCESS );

#ifdef HPL_USE_MPI_DATATYPE
   ierr =      MPI_Type_contiguous( SCOUNT, MPI_DOUBLE, &type );
   if( ierr == MPI_SUCCESS ) {
      ierr =   MPI_Type_commit( &type );
   }
   if( ierr == MPI_SUCCESS ) {
      ierr =   MPI_Send( (void *)(SBUF), 1, type, DEST, STAG, COMM );
   }
   if( ierr == MPI_SUCCESS ) {
      ierr =   MPI_Type_free( &type );
   }
#else
   ierr = MPI_Send( (void *)(SBUF), SCOUNT, MPI_DOUBLE, DEST, STAG, COMM );
#endif

END_TRACE

   return( ( ierr == MPI_SUCCESS ? HPL_SUCCESS : HPL_FAILURE ) ); 
/*
 * End of HPL_send
 */
}
