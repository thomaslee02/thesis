#include "kinematics.h"

double curPos1 = 0.0;
double curPos2 = 0.0;
double curPos3 = 0.0;
double curPos4 = 0.0;
double curPos5 = 0.0;
double curPos6 = 0.0;

boolean PULstat1 = 0;
boolean PULstat2 = 0;
boolean PULstat3 = 0;
boolean PULstat4 = 0;
boolean PULstat5 = 0;
boolean PULstat6 = 0;

void printCurPos(){
  for (int i = 0; i < 6; ++i){
    Serial.println(getCurPos(i));
  }
}

void setCurPos(double pos1, double pos2, double pos3, double pos4, double pos5, double pos6){
  curPos1 = pos1;
  curPos2 = pos2;
  curPos3 = pos3;
  curPos4 = pos4;
  curPos5 = pos5;
  curPos6 = pos6;
}

float getCurPos(uint8_t i){
  switch(i){
    case 0:
      return curPos1;
    case 1:
      return curPos2;
    case 2:
      return curPos3;
    case 3:
      return curPos4;
    case 4:
      return curPos5;
    case 5:
      return curPos6;
    default:
      return 0;
  }
}

void goStrightLine(float* xfi, float* xff, float vel0, float acc0, float velini, float velfin){
  //
  float lmax = max(abs(xff[0]-xfi[0]),abs(xff[1]-xfi[1]));
  lmax = max(lmax,abs(xff[2]-xfi[2]));
  lmax = max(lmax,abs(xff[3]-xfi[3]));
  lmax = max(lmax,abs(xff[4]-xfi[4]));
  lmax = max(lmax,abs(xff[5]-xfi[5]));
  unsigned long preMil = micros();
  double l = 0.0;
  vel0 = min(vel0,sqrt(lmax*acc0+0.5*velini*velini+0.5*velfin*velfin));
  unsigned long curMil = micros();
  unsigned long t = 0;
  double tap = vel0/acc0-velini/acc0;
  double lap = velini*tap+acc0*tap*tap/2.0;
  double lcsp = lmax-(vel0*vel0/2.0/acc0-velfin*velfin/2.0/acc0);
  double tcsp = (lcsp-lap)/vel0+tap;
  double tfin = vel0/acc0-velfin/acc0+tcsp;
  while (curMil-preMil<=tfin){
    t = curMil-preMil;
    //acceleration phase
    if (t<=tap) {
      l = velini*t+acc0*t*t/2.0;
    }
    //contant maximum speed phase
    if (t>tap && t<=tcsp) {
      l = lap+vel0*(t-tap);
    }
    //deceleration phase
    if (t>tcsp) {
      l = lcsp+vel0*(t-tcsp)-acc0*(t-tcsp)*(t-tcsp)/2.0;
    }
  
    //trajectory x and y as a function of l
    float Xx[6];
    Xx[0]=xfi[0]+(xff[0]-xfi[0])/lmax*l;
    Xx[1]=xfi[1]+(xff[1]-xfi[1])/lmax*l;
    Xx[2]=xfi[2]+(xff[2]-xfi[2])/lmax*l;
    Xx[3]=xfi[3]+(xff[3]-xfi[3])/lmax*l;
    Xx[4]=xfi[4]+(xff[4]-xfi[4])/lmax*l;
    Xx[5]=xfi[5]+(xff[5]-xfi[5])/lmax*l;
    
    goTrajectory(Xx);
    curMil = micros();
  }
}

void goTrajectory(float* Jf){
  
  //execution
  int delF=2;
  // joint #1
  if (Jf[0]-curPos1>0.0) { // positive direction of rotation
    digitalWrite(DIR1_PIN, HIGH);
    while (Jf[0]-curPos1>dl1/2.0) {
      if (PULstat1 == 0) {
        digitalWrite(PUL1_PIN, HIGH);
        PULstat1 = 1;
      } else {
        digitalWrite(PUL1_PIN, LOW);
        PULstat1 = 0;
      }
      //curPos1 = Jf[0];
      curPos1 = curPos1 + dl1/2.0;
      if (Jf[0]-curPos1>dl1/2.0) {
        delayMicroseconds(delF);
      }
    }
  } else {
    digitalWrite(DIR1_PIN, LOW);
    while (-Jf[0]+curPos1>dl1/2.0) {
      if (PULstat1 == 0) {
        digitalWrite(PUL1_PIN, HIGH);
        PULstat1 = 1;
      } else {
        digitalWrite(PUL1_PIN, LOW);
        PULstat1 = 0;
      }
      //curPos1 = Jf[0];
      curPos1 = curPos1 - dl1/2.0;
      if (-Jf[0]+curPos1>dl1/2.0) {
        delayMicroseconds(delF);
      }
    }
  }
  // joint #2
  if (Jf[1]-curPos2>0.0) { // positive direction of rotation
    digitalWrite(DIR2_PIN, HIGH);
    while (Jf[1]-curPos2>dl2/2.0) {
      if (PULstat2 == 0) {
        digitalWrite(PUL2_PIN, HIGH);
        PULstat2 = 1;
      } else {
        digitalWrite(PUL2_PIN, LOW);
        PULstat2 = 0;
      }
      //curPos2 = Jf[1];
      curPos2 = curPos2 + dl2/2.0;
      if (Jf[1]-curPos2>dl2/2.0) {
        delayMicroseconds(delF);
      }
    }
  } else {
    digitalWrite(DIR2_PIN, LOW);
    while (-Jf[1]+curPos2>dl2/2.0) {
      if (PULstat2 == 0) {
        digitalWrite(PUL2_PIN, HIGH);
        PULstat2 = 1;
      } else {
        digitalWrite(PUL2_PIN, LOW);
        PULstat2 = 0;
      }
      //curPos2 = Jf[1];
      curPos2 = curPos2 - dl2/2.0;
      if (-Jf[1]+curPos2>dl2/2.0) {
        delayMicroseconds(delF);
      }
    }
  }
  // joint #3
  if (Jf[2]-curPos3>0.0) { // positive direction of rotation
    digitalWrite(DIR3_PIN, HIGH);
    while (Jf[2]-curPos3>dl3/2.0) {
      if (PULstat3 == 0) {
        digitalWrite(PUL3_PIN, HIGH);
        PULstat3 = 1;
      } else {
        digitalWrite(PUL3_PIN, LOW);
        PULstat3 = 0;
      }
      //curPos3 = Jf[2];
      curPos3 = curPos3 + dl3/2.0;
      if (Jf[2]-curPos3>dl3/2.0) {
        delayMicroseconds(delF);
      }
    }
  } else {
    digitalWrite(DIR3_PIN, LOW);
    while (-Jf[2]+curPos3>dl3/2.0) {
      if (PULstat3 == 0) {
        digitalWrite(PUL3_PIN, HIGH);
        PULstat3 = 1;
      } else {
        digitalWrite(PUL3_PIN, LOW);
        PULstat3 = 0;
      }
      //curPos3 = Jf[2];
      curPos3 = curPos3 - dl3/2.0;
      if (-Jf[2]+curPos3>dl3/2.0) {
        delayMicroseconds(delF);
      }
    }
  }
  // joint #4
  if (Jf[3]-curPos4>0.0) { // positive direction of rotation
    digitalWrite(DIR4_PIN, HIGH);
    while (Jf[3]-curPos4>dl4/2.0) {
      if (PULstat4 == 0) {
        digitalWrite(PUL4_PIN, HIGH);
        PULstat4 = 1;
      } else {
        digitalWrite(PUL4_PIN, LOW);
        PULstat4 = 0;
      }
      //curPos4 = Jf[3];
      curPos4 = curPos4 + dl4/2.0;
      if (Jf[3]-curPos4>dl4/2.0) {
        delayMicroseconds(delF);
      }
    }
  } else {
    digitalWrite(DIR4_PIN, LOW);
    while (-Jf[3]+curPos4>dl4/2.0) {
      if (PULstat4 == 0) {
        digitalWrite(PUL4_PIN, HIGH);
        PULstat4 = 1;
      } else {
        digitalWrite(PUL4_PIN, LOW);
        PULstat4 = 0;
      }
      //curPos4 = Jf[3];
      curPos4 = curPos4 - dl4/2.0;
      if (-Jf[3]+curPos4>dl4/2.0) {
        delayMicroseconds(delF);
      }
    }
  }
  // joint #5
  if (Jf[4]-curPos5>0.0) { // positive direction of rotation
    digitalWrite(DIR5_PIN, HIGH);
    while (Jf[4]-curPos5>dl5/2.0) {
      if (PULstat5 == 0) {
        digitalWrite(PUL5_PIN, HIGH);
        PULstat5 = 1;
      } else {
        digitalWrite(PUL5_PIN, LOW);
        PULstat5 = 0;
      }
      //curPos5 = Jf[4];
      curPos5 = curPos5 + dl5/2.0;
      if (Jf[4]-curPos5>dl5/2.0) {
        delayMicroseconds(delF);
      }
    }
  } else {
    digitalWrite(DIR5_PIN, LOW);
    while (-Jf[4]+curPos5>dl5/2.0) {
      if (PULstat5 == 0) {
        digitalWrite(PUL5_PIN, HIGH);
        PULstat5 = 1;
      } else {
        digitalWrite(PUL5_PIN, LOW);
        PULstat5 = 0;
      }
      //curPos5 = Jf[4];
      curPos5 = curPos5 - dl5/2.0;
      if (-Jf[4]+curPos5>dl5/2.0) {
        delayMicroseconds(delF);
      }
    }
  }
  // joint #6
  if (Jf[5]-curPos6>0.0) { // positive direction of rotation
    digitalWrite(DIR6_PIN, HIGH);
    while (Jf[5]-curPos6>dl6/2.0) {
      if (PULstat6 == 0) {
        digitalWrite(PUL6_PIN, HIGH);
        PULstat6 = 1;
      } else {
        digitalWrite(PUL6_PIN, LOW);
        PULstat6 = 0;
      }
      //curPos6 = Jf[5];
      curPos6 = curPos6 + dl6/2.0;
      if (Jf[5]-curPos6>dl6/2.0) {
        delayMicroseconds(delF);
      }
    }
  } else {
    digitalWrite(DIR6_PIN, LOW);
    while (-Jf[5]+curPos6>dl6/2.0) {
      if (PULstat6 == 0) {
        digitalWrite(PUL6_PIN, HIGH);
        PULstat6 = 1;
      } else {
        digitalWrite(PUL6_PIN, LOW);
        PULstat6 = 0;
      }
      //curPos6 = Jf[5];
      curPos6 = curPos6 - dl6/2.0;
      if (-Jf[5]+curPos6>dl6/2.0) {
        delayMicroseconds(delF);
      }
    }
  }
}

void InverseK(float* Xik, float* Jik)
{
  // inverse kinematics
  // input: Xik - pos value for the calculation of the inverse kinematics
  // output: Jfk - joints value for the calculation of the inversed kinematics
  
  // from deg to rad
  // Xik(4:6)=Xik(4:6)*pi/180;
  Xik[3]=Xik[3]*PI/180.0;
  Xik[4]=Xik[4]*PI/180.0;
  Xik[5]=Xik[5]*PI/180.0;
  
  // Denavit-Hartenberg matrix
  float theta[6]={0.0, -90.0, 0.0, 0.0, 0.0, 0.0}; // theta=[0; -90+0; 0; 0; 0; 0];
  float alfa[6]={-90.0, 0.0, -90.0, 90.0, -90.0, 0.0}; // alfa=[-90; 0; -90; 90; -90; 0];
  float r[6]={r1, r2, r3, 0.0, 0.0, 0.0}; // r=[47; 110; 26; 0; 0; 0];
  float d[6]={d1, 0.0, d3, d4, 0.0, d6}; // d=[133; 0; 7; 117.5; 0; 28];
  // from deg to rad
  MatrixScale(theta, 6, 1, PI/180.0); // theta=theta*pi/180;
  MatrixScale(alfa, 6, 1, PI/180.0); // alfa=alfa*pi/180;
  
  // work frame
  float Xwf[6]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // Xwf=[0; 0; 0; 0; 0; 0];
  
  // tool frame
  float Xtf[6]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // Xtf=[0; 0; 0; 0; 0; 0];
  
  // work frame transformation matrix
  float Twf[16];
  pos2tran(Xwf, Twf); // Twf=pos2tran(Xwf);
  
  // tool frame transformation matrix
  float Ttf[16];
  pos2tran(Xtf, Ttf); // Ttf=pos2tran(Xtf);
  
  // total transformation matrix
  float Twt[16];
  pos2tran(Xik, Twt); // Twt=pos2tran(Xik);
  
  // find T06
  float inTwf[16], inTtf[16], Tw6[16], T06[16];
  invtran(Twf, inTwf); // inTwf=invtran(Twf);
  invtran(Ttf, inTtf); // inTtf=invtran(Ttf);
  MatrixMultiply(Twt, inTtf, 4, 4, 4, Tw6); // Tw6=Twt*inTtf;
  MatrixMultiply(inTwf, Tw6, 4, 4, 4, T06); // T06=inTwf*Tw6;
  
  // positon of the spherical wrist
  float Xsw[3];
  // Xsw=T06(1:3,4)-d(6)*T06(1:3,3);
  Xsw[0]=T06[0*4 + 3]-d[5]*T06[0*4 + 2];
  Xsw[1]=T06[1*4 + 3]-d[5]*T06[1*4 + 2];
  Xsw[2]=T06[2*4 + 3]-d[5]*T06[2*4 + 2];
  
  // joints variable
  // Jik=zeros(6,1);
  // first joint
  Jik[0]=atan2(Xsw[1],Xsw[0])-atan2(d[2],sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])); // Jik(1)=atan2(Xsw(2),Xsw(1))-atan2(d(3),sqrt(Xsw(1)^2+Xsw(2)^2-d(3)^2));
  // second joint
  Jik[1]=PI/2.0
  -acos((r[1]*r[1]+(Xsw[2]-d[0])*(Xsw[2]-d[0])+(sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])-r[0])*(sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])-r[0])-(r[2]*r[2]+d[3]*d[3]))/(2.0*r[1]*sqrt((Xsw[2]-d[0])*(Xsw[2]-d[0])+(sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])-r[0])*(sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])-r[0]))))
  -atan((Xsw[2]-d[0])/(sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])-r[0])); // Jik(2)=pi/2-acos((r(2)^2+(Xsw(3)-d(1))^2+(sqrt(Xsw(1)^2+Xsw(2)^2-d(3)^2)-r(1))^2-(r(3)^2+d(4)^2))/(2*r(2)*sqrt((Xsw(3)-d(1))^2+(sqrt(Xsw(1)^2+Xsw(2)^2-d(3)^2)-r(1))^2)))-atan((Xsw(3)-d(1))/(sqrt(Xsw(1)^2+Xsw(2)^2-d(3)^2)-r(1)));
  // third joint
  Jik[2]=PI
  -acos((r[1]*r[1]+r[2]*r[2]+d[3]*d[3]-(Xsw[2]-d[0])*(Xsw[2]-d[0])-(sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])-r[0])*(sqrt(Xsw[0]*Xsw[0]+Xsw[1]*Xsw[1]-d[2]*d[2])-r[0]))/(2*r[1]*sqrt(r[2]*r[2]+d[3]*d[3])))
  -atan(d[3]/r[2]); // Jik(3)=pi-acos((r(2)^2+r(3)^2+d(4)^2-(Xsw(3)-d(1))^2-(sqrt(Xsw(1)^2+Xsw(2)^2-d(3)^2)-r(1))^2)/(2*r(2)*sqrt(r(3)^2+d(4)^2)))-atan(d(4)/r(3));
  // last three joints
  float T01[16], T12[16], T23[16], T02[16], T03[16], inT03[16], T36[16];
  DH1line(theta[0]+Jik[0], alfa[0], r[0], d[0], T01); // T01=DH1line(theta(1)+Jik(1),alfa(1),r(1),d(1));
  DH1line(theta[1]+Jik[1], alfa[1], r[1], d[1], T12); // T12=DH1line(theta(2)+Jik(2),alfa(2),r(2),d(2));
  DH1line(theta[2]+Jik[2], alfa[2], r[2], d[2], T23); // T23=DH1line(theta(3)+Jik(3),alfa(3),r(3),d(3));
  MatrixMultiply(T01, T12, 4, 4, 4, T02); // T02=T01*T12;
  MatrixMultiply(T02, T23, 4, 4, 4, T03); // T03=T02*T23;
  invtran(T03, inT03); // inT03=invtran(T03);
  MatrixMultiply(inT03, T06, 4, 4, 4, T36); // T36=inT03*T06;
  // forth joint
  Jik[3]=atan2(-T36[1*4+2], -T36[0*4+2]); // Jik(4)=atan2(-T36(2,3),-T36(1,3));
  // fifth joint
  Jik[4]=atan2(sqrt(T36[0*4+2]*T36[0*4+2]+T36[1*4+2]*T36[1*4+2]), T36[2*4+2]); // Jik(5)=atan2(sqrt(T36(1,3)^2+T36(2,3)^2),T36(3,3));
  // sixth joints
  Jik[5]=atan2(-T36[2*4+1], T36[2*4+0]); // Jik(6)=atan2(-T36(3,2),T36(3,1));
  // rad to deg
  MatrixScale(Jik, 6, 1, 180.0/PI); // Jik=Jik/pi*180;
}

void ForwardK(float* Jfk, float* Xfk)
{
  // forward kinematics
  // input: Jfk - joints value for the calculation of the forward kinematics
  // output: Xfk - pos value for the calculation of the forward kinematics
  
  // Denavit-Hartenberg matrix
  float theTemp[6]={0.0, -90.0, 0.0, 0.0, 0.0, 0.0};
  float theta[6];
  MatrixAdd(theTemp, Jfk, 6, 1, theta); // theta=[Jfk(1); -90+Jfk(2); Jfk(3); Jfk(4); Jfk(5); Jfk(6)];
  float alfa[6]={-90.0, 0.0, -90.0, 90.0, -90.0, 0.0}; // alfa=[-90; 0; -90; 90; -90; 0];
  float r[6]={r1, r2, r3, 0.0, 0.0, 0.0}; // r=[47; 110; 26; 0; 0; 0];
  float d[6]={d1, 0.0, d3, d4, 0.0, d6}; // d=[133; 0; 7; 117.5; 0; 28];
  // from deg to rad
  MatrixScale(theta, 6, 1, PI/180.0); // theta=theta*pi/180;
  MatrixScale(alfa, 6, 1, PI/180.0); // alfa=alfa*pi/180;
  
  // work frame
  float Xwf[6]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // Xwf=[0; 0; 0; 0; 0; 0];
  
  // tool frame
  float Xtf[6]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // Xtf=[0; 0; 0; 0; 0; 0];
  
  // work frame transformation matrix
  float Twf[16];
  pos2tran(Xwf, Twf); // Twf=pos2tran(Xwf);
  
  // tool frame transformation matrix
  float Ttf[16];
  pos2tran(Xtf, Ttf); // Ttf=pos2tran(Xtf);
  
  // DH homogeneous transformation matrix
  float T01[16], T12[16], T23[16], T34[16], T45[16], T56[16];
  DH1line(theta[0], alfa[0], r[0], d[0], T01); // T01=DH1line(theta(1),alfa(1),r(1),d(1));
  DH1line(theta[1], alfa[1], r[1], d[1], T12); // T12=DH1line(theta(2),alfa(2),r(2),d(2));
  DH1line(theta[2], alfa[2], r[2], d[2], T23); // T23=DH1line(theta(3),alfa(3),r(3),d(3));
  DH1line(theta[3], alfa[3], r[3], d[3], T34); // T34=DH1line(theta(4),alfa(4),r(4),d(4));
  DH1line(theta[4], alfa[4], r[4], d[4], T45); // T45=DH1line(theta(5),alfa(5),r(5),d(5));
  DH1line(theta[5], alfa[5], r[5], d[5], T56); // T56=DH1line(theta(6),alfa(6),r(6),d(6));

  float Tw1[16], Tw2[16], Tw3[16], Tw4[16], Tw5[16], Tw6[16], Twt[16];
  MatrixMultiply(Twf, T01, 4, 4, 4, Tw1); // Tw1=Twf*T01;
  MatrixMultiply(Tw1, T12, 4, 4, 4, Tw2); // Tw2=Tw1*T12;
  MatrixMultiply(Tw2, T23, 4, 4, 4, Tw3); // Tw3=Tw2*T23;
  MatrixMultiply(Tw3, T34, 4, 4, 4, Tw4); // Tw4=Tw3*T34;
  MatrixMultiply(Tw4, T45, 4, 4, 4, Tw5); // Tw5=Tw4*T45;
  MatrixMultiply(Tw5, T56, 4, 4, 4, Tw6); // Tw6=Tw5*T56;
  MatrixMultiply(Tw6, Ttf, 4, 4, 4, Twt); // Twt=Tw6*Ttf;
  
  // calculate pos from transformation matrix
  tran2pos(Twt, Xfk); // Xfk=tran2pos(Twt);
  // Xfk(4:6)=Xfk(4:6)/pi*180;
  Xfk[3]=Xfk[3]/PI*180.0;
  Xfk[4]=Xfk[4]/PI*180.0;
  Xfk[5]=Xfk[5]/PI*180.0;
}

void invtran(float* Titi, float* Titf)
{
  // finding the inverse of the homogeneous transformation matrix
  // first row
  Titf[0*4 + 0] = Titi[0*4 + 0];
  Titf[0*4 + 1] = Titi[1*4 + 0];
  Titf[0*4 + 2] = Titi[2*4 + 0];
  Titf[0*4 + 3] = -Titi[0*4 + 0]*Titi[0*4 + 3]-Titi[1*4 + 0]*Titi[1*4 + 3]-Titi[2*4 + 0]*Titi[2*4 + 3];
  // second row
  Titf[1*4 + 0] = Titi[0*4 + 1];
  Titf[1*4 + 1] = Titi[1*4 + 1];
  Titf[1*4 + 2] = Titi[2*4 + 1];
  Titf[1*4 + 3] = -Titi[0*4 + 1]*Titi[0*4 + 3]-Titi[1*4 + 1]*Titi[1*4 + 3]-Titi[2*4 + 1]*Titi[2*4 + 3];
  // third row
  Titf[2*4 + 0] = Titi[0*4 + 2];
  Titf[2*4 + 1] = Titi[1*4 + 2];
  Titf[2*4 + 2] = Titi[2*4 + 2];
  Titf[2*4 + 3] = -Titi[0*4 + 2]*Titi[0*4 + 3]-Titi[1*4 + 2]*Titi[1*4 + 3]-Titi[2*4 + 2]*Titi[2*4 + 3];
  // forth row
  Titf[3*4 + 0] = 0.0;
  Titf[3*4 + 1] = 0.0;
  Titf[3*4 + 2] = 0.0;
  Titf[3*4 + 3] = 1.0;
}

void tran2pos(float* Ttp, float* Xtp)
{
  // pos from homogeneous transformation matrix
  Xtp[0] = Ttp[0*4 + 3];
  Xtp[1] = Ttp[1*4 + 3];
  Xtp[2] = Ttp[2*4 + 3];
  Xtp[4] = atan2(sqrt(Ttp[2*4 + 0]*Ttp[2*4 + 0] + Ttp[2*4 + 1]*Ttp[2*4 + 1]),Ttp[2*4 + 2]);
  Xtp[3] = atan2(Ttp[1*4 + 2]/sin(Xtp[4]),Ttp[0*4 + 2]/sin(Xtp[4]));
  Xtp[5] = atan2(Ttp[2*4 + 1]/sin(Xtp[4]),-Ttp[2*4 + 0]/sin(Xtp[4]));
}

void pos2tran(float* Xpt, float* Tpt)
{
  // pos to homogeneous transformation matrix
  // first row
  Tpt[0*4 + 0] = cos(Xpt[3])*cos(Xpt[4])*cos(Xpt[5])-sin(Xpt[3])*sin(Xpt[5]);
  Tpt[0*4 + 1] = -cos(Xpt[3])*cos(Xpt[4])*sin(Xpt[5])-sin(Xpt[3])*cos(Xpt[5]);
  Tpt[0*4 + 2] = cos(Xpt[3])*sin(Xpt[4]);
  Tpt[0*4 + 3] = Xpt[0];
  // second row
  Tpt[1*4 + 0] = sin(Xpt[3])*cos(Xpt[4])*cos(Xpt[5])+cos(Xpt[3])*sin(Xpt[5]);
  Tpt[1*4 + 1] = -sin(Xpt[3])*cos(Xpt[4])*sin(Xpt[5])+cos(Xpt[3])*cos(Xpt[5]);
  Tpt[1*4 + 2] = sin(Xpt[3])*sin(Xpt[4]);
  Tpt[1*4 + 3] = Xpt[1];
  // third row
  Tpt[2*4 + 0] = -sin(Xpt[4])*cos(Xpt[5]);
  Tpt[2*4 + 1] = sin(Xpt[4])*sin(Xpt[5]);
  Tpt[2*4 + 2] = cos(Xpt[4]);
  Tpt[2*4 + 3] = Xpt[2];
  // forth row
  Tpt[3*4 + 0] = 0.0;
  Tpt[3*4 + 1] = 0.0;
  Tpt[3*4 + 2] = 0.0;
  Tpt[3*4 + 3] = 1.0;
}

void DH1line(float thetadh, float alfadh, float rdh, float ddh, float* Tdh)
{
  // creats Denavit-Hartenberg homogeneous transformation matrix
  // first row
  Tdh[0*4 + 0] = cos(thetadh);
  Tdh[0*4 + 1] = -sin(thetadh)*cos(alfadh);
  Tdh[0*4 + 2] = sin(thetadh)*sin(alfadh);
  Tdh[0*4 + 3] = rdh*cos(thetadh);
  // second row
  Tdh[1*4 + 0] = sin(thetadh);
  Tdh[1*4 + 1] = cos(thetadh)*cos(alfadh);
  Tdh[1*4 + 2] = -cos(thetadh)*sin(alfadh);
  Tdh[1*4 + 3] = rdh*sin(thetadh);
  // third row
  Tdh[2*4 + 0] = 0.0;
  Tdh[2*4 + 1] = sin(alfadh);
  Tdh[2*4 + 2] = cos(alfadh);
  Tdh[2*4 + 3] = ddh;
  // forth row
  Tdh[3*4 + 0] = 0.0;
  Tdh[3*4 + 1] = 0.0;
  Tdh[3*4 + 2] = 0.0;
  Tdh[3*4 + 3] = 1.0;
}

void MatrixPrint(float* A, int m, int n, String label)
{
  // A = input matrix (m x n)
  int i, j;
  Serial.println();
  Serial.println(label);
  for (i = 0; i < m; i++)
  {
    for (j = 0; j < n; j++)
    {
      Serial.print(A[n * i + j]);
      Serial.print("\t");
    }
    Serial.println();
  }
}

void MatrixCopy(float* A, int n, int m, float* B)
{
  int i, j;
  for (i = 0; i < m; i++)
    for(j = 0; j < n; j++)
    {
      B[n * i + j] = A[n * i + j];
    }
}

//Matrix Multiplication Routine
// C = A*B
void MatrixMultiply(float* A, float* B, int m, int p, int n, float* C)
{
  // A = input matrix (m x p)
  // B = input matrix (p x n)
  // m = number of rows in A
  // p = number of columns in A = number of rows in B
  // n = number of columns in B
  // C = output matrix = A*B (m x n)
  int i, j, k;
  for (i = 0; i < m; i++)
    for(j = 0; j < n; j++)
    {
      C[n * i + j] = 0;
      for (k = 0; k < p; k++)
        C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
    }
}


//Matrix Addition Routine
void MatrixAdd(float* A, float* B, int m, int n, float* C)
{
  // A = input matrix (m x n)
  // B = input matrix (m x n)
  // m = number of rows in A = number of rows in B
  // n = number of columns in A = number of columns in B
  // C = output matrix = A+B (m x n)
  int i, j;
  for (i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      C[n * i + j] = A[n * i + j] + B[n * i + j];
}


//Matrix Subtraction Routine
void MatrixSubtract(float* A, float* B, int m, int n, float* C)
{
  // A = input matrix (m x n)
  // B = input matrix (m x n)
  // m = number of rows in A = number of rows in B
  // n = number of columns in A = number of columns in B
  // C = output matrix = A-B (m x n)
  int i, j;
  for (i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      C[n * i + j] = A[n * i + j] - B[n * i + j];
}


//Matrix Transpose Routine
void MatrixTranspose(float* A, int m, int n, float* C)
{
  // A = input matrix (m x n)
  // m = number of rows in A
  // n = number of columns in A
  // C = output matrix = the transpose of A (n x m)
  int i, j;
  for (i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      C[m * j + i] = A[n * i + j];
}

void MatrixScale(float* A, int m, int n, float k)
{
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      A[n * i + j] = A[n * i + j] * k;
}
