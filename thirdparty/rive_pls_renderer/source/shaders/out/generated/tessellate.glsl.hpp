#pragma once

#include "tessellate.exports.h"

namespace rive {
namespace pls {
namespace glsl {
const char tessellate[] = R"===(#define ta 10
#ifdef V
U0(P)q0(0,g,GC);q0(1,g,HC);q0(2,g,ZB);q0(3,T,NB);V0
#endif
A1 k0 I(0,g,C3);k0 I(1,g,D3);k0 I(2,g,A2);k0 I(3,D1,e3);d6 I(4,uint,l4);B1 p A i8(d T0,d r0,d I0,d f1){A t;t[0]=(any(notEqual(T0,r0))?r0:any(notEqual(r0,I0))?I0:f1)-T0;t[1]=f1-(any(notEqual(f1,I0))?I0:any(notEqual(I0,r0))?r0:T0);return t;}
#ifdef V
O1 P1 V1 R2(w7,B9,SB);R2(x7,C9,FC);W1 float j8(d m,d b){float ua=dot(m,b);float k8=dot(m,m)*dot(b,b);return(k8==.0)?1.:clamp(ua*inversesqrt(k8),-1.,1.);}g1(UD,P,r,j,L){v0(L,r,GC,g);v0(L,r,HC,g);v0(L,r,ZB,g);v0(L,r,NB,T);Q(C3,g);Q(D3,g);Q(A2,g);Q(e3,D1);Q(l4,uint);d T0=GC.xy;d r0=GC.zw;d I0=HC.xy;d f1=HC.zw;bool l8=j<4;float y=l8?ZB.z:ZB.w;int m8=int(l8?NB.x:NB.y);float m6=float(m8<<16>>16);float X4=float(m8>>16);d j1=d((j&1)==0?m6:X4,(j&2)==0?y+1.:y);uint v1=NB.z&0x3ffu;uint n8=(NB.z>>10)&0x3ffu;uint f3=NB.z>>20;uint D=NB.w;if(X4<m6){D|=I4;}if((X4-m6)*v.F5<.0){j1.y=2.*y+1.-j1.y;}if((D&q9)!=0u){uint va=z0(FC,Z6(D)).z;A o8=W0(uintBitsToFloat(z0(SB,va*2u)));d p8=h0(o8,-2.*r0+I0+T0);d q8=h0(o8,-2.*I0+f1+r0);float e1=max(dot(p8,p8),dot(q8,q8));float E2=max(ceil(sqrt(.75*4.*sqrt(e1))),1.);v1=min(uint(E2),v1);}uint Y4=v1+n8+f3-1u;A U1=i8(T0,r0,I0,f1);float p1=acos(j8(U1[0],U1[1]));float g2=p1/float(n8);float n6=determinant(A(I0-T0,f1-r0));if(n6==.0)n6=determinant(U1);if(n6<.0)g2=-g2;C3=g(T0,r0);D3=g(I0,f1);A2=g(float(Y4)-abs(X4-j1.x),float(Y4),(f3<<10)|v1,g2);if(f3>1u){A o6=A(U1[1],ZB.xy);float wa=acos(j8(o6[0],o6[1]));float r8=float(f3);if((D&(Z3|H4))==H4){r8-=2.;}float p6=wa/r8;if(determinant(o6)<.0)p6=-p6;e3.xy=ZB.xy;e3.z=p6;}l4=D;g B;B.x=j1.x*(2./p9)-1.;B.y=j1.y*v.F5-sign(v.F5);B.zw=d(0,1);S(C3);S(D3);S(A2);S(e3);S(l4);h1(B);}
#endif
#ifdef GB
r2(T,VD){N(C3,g);N(D3,g);N(A2,g);N(e3,D1);N(l4,uint);d T0=C3.xy;d r0=C3.zw;d I0=D3.xy;d f1=D3.zw;A U1=i8(T0,r0,I0,f1);float xa=max(floor(A2.x),.0);float Y4=A2.y;uint v8=uint(A2.z);float v1=float(v8&0x3ffu);float f3=float(v8>>10);float g2=A2.w;uint D=l4;float g3=Y4-f3;float w1=xa;if(w1<=g3){D&=~Z3;}else{T0=r0=I0=f1;U1=A(U1[1],e3.xy);v1=1.;w1-=g3;g3=f3;if((D&Z3)!=0u){if(w1<2.5)D|=L5;if(w1>1.5&&w1<3.5)D|=n7;}else if((D&H4)!=0u){g3-=2.;w1--;}g2=e3.z;D|=g2<.0?J4:o7;}d Z4;float p1=.0;if(w1==.0||w1==g3||(D&Z3)!=0u){bool R4=w1<g3*.5;Z4=R4?T0:f1;p1=c7(R4?U1[0]:U1[1]);}else if((D&m7)!=0u){Z4=r0;}else{float T1,h3;if(v1==g3){T1=w1/v1;h3=.0;}else{d d0,g0,T4=r0-T0;d a8=f1-T0;d w8=I0-r0;g0=w8-T4;d0=-3.*w8+a8;d ya=g0*(v1*2.);d za=T4*(v1*v1);float a5=.0;float Aa=min(v1-1.,w1);d q6=normalize(U1[0]);float Ba=-abs(g2);float Ca=(1.+w1)*abs(g2);for(int G3=ta-1;G3>=0;--G3){float m4=a5+exp2(float(G3));if(m4<=Aa){d r6=m4*d0+ya;r6=m4*r6+za;float Da=dot(normalize(r6),q6);float v6=m4*Ba+Ca;v6=min(v6,E4);if(Da>=cos(v6))a5=m4;}}float Ea=a5/v1;float x8=w1-a5;float c5=acos(clamp(q6.x,-1.,1.));c5=q6.y>=.0?c5:-c5;p1=x8*g2+c5;d V2=d(sin(p1),-cos(p1));float m=dot(V2,d0),d5=dot(V2,g0),e0=dot(V2,T4);float Fa=max(d5*d5-m*e0,.0);float B2=sqrt(Fa);if(d5>.0)B2=-B2;B2-=d5;float y8=-.5*B2*m;d w6=(abs(B2*B2+y8)<abs(m*e0+y8))?d(B2,m):d(e0,B2);h3=(w6.y!=.0)?w6.x/w6.y:.0;h3=clamp(h3,.0,1.);if(x8==.0)h3=.0;T1=max(Ea,h3);}d Ga=r3(T0,r0,T1);d z8=r3(r0,I0,T1);d Ha=r3(I0,f1,T1);d A8=r3(Ga,z8,T1);d B8=r3(z8,Ha,T1);Z4=r3(A8,B8,T1);if(T1!=h3)p1=c7(B8-A8);}v2(T(floatBitsToUint(D1(Z4,p1)),D));}
#endif
)===";
} // namespace glsl
} // namespace pls
} // namespace rive