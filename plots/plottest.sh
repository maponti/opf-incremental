gnuplot <<- EOF
        set xlabel ""
        set ylabel ""
        set term png
        set title "" 
        set output "$2"
        plot "$1" using (\$2 == 1 ? \$4 : 1/0):(\$2 == 1 ? \$5 : 1/0) every ::1 pt 2 lt 1 title "", \
                 "$1" using (\$2 == 2 ? \$4 : 1/0):(\$2 == 2 ? \$5 : 1/0) every ::1 pt 2 lt 2 title "", \
                 "$1" using (\$2 == 3 ? \$4 : 1/0):(\$2 == 3 ? \$5 : 1/0) every ::1 pt 2 lt 3 title "", \
                 "$1" using (\$2 == 4 ? \$4 : 1/0):(\$2 == 4 ? \$5 : 1/0) every ::1 pt 2 lt 4 title ""
EOF
