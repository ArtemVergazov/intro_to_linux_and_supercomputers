#!/usr/bin/bash


awk '/MASTER/{ nc=$4; cc=$6;
               if(cc > 0) {line++; n+=nc; circ+=cc;}
               pi = 4.0*circ/n; dpi = pi*sqrt(2.0/n);
               printf "\rline: %10d pi: %f +- %g  ",line,pi,dpi;
               system("sleep 0.01");
             }
    END{ pi = 4.0*circ/n; dpi = pi*sqrt(2.0/n);
               printf "\rline: %10d pi: %f +- %g\n",line,pi,dpi;
       } ' < log
