#!perl -w
#
#   A GNU flex wrapper which fixes the output source for compatibility with standard C++.
#   Reported errors are also reformatted to allow double-click-to-error-line
#   in visual studio.
#   Using simple regexps, some unixisms or pre-standard/archaic C++-isms
#   are substibuted with correct code.
#
#   (C) Ivan Vecerina, 2002   --   http://ivan.vecerina.com
#
#   This file is part of delphi2cpp, written by Ivan Vecerina, (C) 2003-2007.
#
#   delphi2cpp is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   delphi2cpp is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with delphi2cpp.  If not, see <http://www.gnu.org/licenses/>.
#
use diagnostics;
use warnings;
use strict;


sub extractParam(\@$)
{
   my @a = @{$_[0]};
   my $h = $_[1];
   my( @pre, $ans );

   while( @a )
   {
      $ans = shift @a;
      if( substr( $ans, 0, length($h) ) eq $h )
      {
         @{$_[0]} = ( @pre, @a );   # assign all but the matched item
         return substr( $ans, length($h) );
      }

      push @pre, $ans;
   }

   return undef;
}


my $src = pop @ARGV;   # file is last parameter, according to usual flex syntax

my $out = extractParam(@ARGV, '-o' );
$out = 'lex.yy.c' unless defined $out;

my $errF = "${out}.err~"; # temp file to capture error output
my $outF = "${out}.out~"; # temp file for intermediate output

my $cmd = 'D:\cygwin\bin\flex -t';
$cmd .= ' "' . join( '" "', @ARGV ) . '"'  if (@ARGV);
$cmd .= " -o$out $src ";

my $flexErrCode = ( system "$cmd 2>$errF 1>$outF" ) >> 8;

if (1) { ## always forward the program's error output, with proper filtering
   if( $flexErrCode )
     { print STDERR "Flex reported errors ($? $!) :\n"; }

   # Format error+line info to allow double-clicking in MSVC

   open( ERR, "<$errF" ) or die "Could not open flex error output: $!";
   while (<ERR>) {
      chomp;
      s@^"(.*)", line ([0-9]+):(.*)$@$1($2) : $3@;
      print STDERR "$_\n";
   }
   close ERR;
}

if ( ! $flexErrCode )
{
   open( SRC, "< $outF" )  or die "can't open intermediate file $outF: $!";
   open( DST, "> $out" )   or die "can't open output file $out: $!";
   my $date = localtime;
   print DST "/* flex-generated file modified by flex_MSVC.pl on $date*/\n";
   print DST "#pragma warning( disable: 4005 ) // unreferenced label\n";
   print DST "#pragma warning( disable: 4102 ) // unreferenced label\n";
   print DST "namespace std {} using namespace std;\n";
   my $dstLine = 5;
   my $shiftLinesFile = $out;
   $shiftLinesFile =~ s#\\#\\\\#g;

   while (<SRC>) {
      ++$dstLine;

      s@class istream;@namespace std { class istream; } // flex_MSVC fixed forward-decl  @;
      s@^(\#include[\s]*<unistd.h>.*)$@//$1 -- disabled by flex_MSVC@;
      s@^(#define __cplusplus)$@//$1 -- disabled by flex_MSVC@;
      s@<stdio.h>@<cstdio> // flex_MSVC fix: was <stdio.h>@;
      s@<stdlib.h>@<cstdlib> // flex_MSVC fix: was <stdlib.h>@;
      if( /^(#line[\s]+)[\d]+([\s]+"(.*?[^\\])".*)$/ ) {  ## fix line numbers
         if( $3 eq $shiftLinesFile ) {
            $_ = "$1$dstLine$2\n";
         }
      }
      print DST;
   }
   close DST  or print STDERR "Error while closing flex_MSVC output file: $!";
}


exit $flexErrCode;

