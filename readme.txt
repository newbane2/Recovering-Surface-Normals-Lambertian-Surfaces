to run s1
./s1 {input original image} {input threshold value} {output parameters file}
./s1 sphere0.pgm 90 s1_out.txt

to run s2
./s2 {input parameters file} {image 1} {image 2} {image 3} {output directions file}
./s2 s1_out.txt sphere1.pgm sphere2.pgm sphere3.pgm s2_output.txt

to run s3
./s3 {input directions} {image 1} {image 2} {image 3} {step} {threshold} {output}
./s3 s2_output.txt object1.pgm object2.pgm object3.pgm 10 85 s3_out.pgm

to run s4
./s4 {input directions} {image 1} {image 2} {image 3} {threshold} {output}
./s4 s2_output.txt object1.pgm object2.pgm object3.pgm 85 s4_out.pgm

Equation for part b:
Normal Vector = [(Xb-Xc) (Yb - Yc) (Zb - Zc)]' (' being transform)
You can solve for Z using the equation
(Zb-Zc)^2 = sqrt(r^2 - (Xb-Xc)^2 - (Yb-Yc)^2)
