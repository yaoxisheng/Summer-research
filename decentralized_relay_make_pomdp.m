function [P,Q,R,T]=decentralized_relay_make_pomdp(p,q,E12,E13,E23,N,fname,lambda)

% parameter N means queue sizes 0 to N
% Actions W_W, E12_W, E13_W, W_E23, E12_E23, E13_E23
Ns=(N+1)*(N+1); % number of states
P=[];
R=[];
O=[];

[Pc,Rc]=centralized_relay(p,q,E12,E13,E23,N,fname,lambda);
% we can multiply Rc with (1-lambda) if we want normalized costs
% careful! don't do it twice...
Rc=(1-lambda)*Rc;
Rc=-Rc; % for maximization/minimization


count=-1;
% decided to drop the 0 threshold...
for i=1:N+1   % i=0 means no W, i=N+1 means always W
  for j=i:N+1 % j=i means no E12, j=N+1 means no E13
    % decided to drop th 0 threshold...
    for k=1:N+1 % k=0 means no W, k=N+1 means always W
      count=count+1;
      for x=0:N % starting state
      for y=0:N % starting state
        if x<i
          u1=0;
        elseif x >=i && x<j
          u1=1;
        else
          u1=2;
        end
        if y<k
          u2=0;
        else
          u2=1;
        end
        u=u2*3+u1; % 0-->W_W to 5-->E13_E23

        P(count+1,x*(N+1)+y+1,:) = Pc(u+1,x*(N+1)+y+1,:);
        Q(count+1,x*(N+1)+y+1,:)=zeros(1,6);
        Q(count+1,x*(N+1)+y+1,u+1)=1;
        R(count+1,x*(N+1)+y+1)= Rc(u+1,x*(N+1)+y+1);
      end
      end
    end
  end
end

Na=count+1;


T=zeros(Na,6,Ns,Ns);
for a=1:Na,
for z=1:6,
for s=1:Ns,
for ss=1:Ns,
  T(a,z,s,ss)=P(a,s,ss)*Q(a,s,z);
end
end
end
end 



fid=fopen(fname,'w');
fprintf(fid,"%d %d %d\n\n",Ns,Na,6);
for a=1:Na,
for z=1:6,
for s=1:Ns,
for ss=1:Ns,
  fprintf(fid,"%f ",T(a,z,s,ss));
end
fprintf(fid,"\n");
end
fprintf(fid,"\n");
end
fprintf(fid,"\n");
end


fprintf(fid,"\n\n");

for a=1:Na,
for s=1:Ns,
  fprintf(fid,"%f ",R(a,s));
end
fprintf(fid,"\n");
end

fclose(fid);

return
