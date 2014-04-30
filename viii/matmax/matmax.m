
A = rand(200);
B = rand(200);
C = rand(200);

tic;
C = A*B+C
toc
fprintf('time %d',toc)
