#include "asservissement.h"

static coordonne positionprecedent;
static coordonne asservConsigne;
static coordonne robot;
static coordonne saveTournerAvancer;
static double vittesseAngulaire;
static double oldErreurVitesseAngulaire;
static double integralVitesseAngulaire;
static double oldconsignevitesseangulaire;
static double oldErreurPositionAngulaire;
static double integralPositionAngulaire;
static double consignevitesseangulaire;
static double erreurVitesseAngulaire;
static double deriveVitesseAngulaire;
static double pidVitesseAngulaire;
static double erreurPosistionAngulaire;
static double derivePositionAngulaire;

static double vitesseLineaire;
static double oldErreurVitesseLineaire;
static double consigneVitesseLineaire;
static double erreurVitesseLineaire;
static double integralVitesseLineaire;
static double deriveVitesseLineaire;
static double pidVitesseLineaire;
static double erreurPosistionLineaire;
static double oldErreurPositionLineaire;
static double oldConsignevitesseLineaire;
static double derivePositionLineaire;
static double integralPositionLineaire;
static double pidPostionLineaire;

static double erreurPositionAngulairePoint;
static double erreurPositionLineairePoint;
static double distanceRobotPoint;
static double freezeAngle;
static double angleAsserv;
static double angleTournerAvancer;

static double angleTournerPoint;

TYPEASSERVE consigneAsservissement;
MODETOURNERAVANCER modeTournerAvancer;

void (*callbackAsserv)(void);
bool callbackinitialiseAsserv = false;


void (*callbackAsservDistance)(int);
bool callbackinitialiseAsservDistance = false;



void setConsigneLineaire(coordonne consigne){
    asservConsigne = consigne;
    consigneAsservissement = AVANCER;
    resetAsserv();
    /* Start timer. */
	TIM_CR1(TIM3) |= TIM_CR1_CEN;
    disableBrakeMoteurDroit();
    disableBrakeMoteurGauche();

}

void setConsigneAngulaire(coordonne consigne){
    asservConsigne = consigne;
    consigneAsservissement = TOURNER;
    resetAsserv();
    /* Start timer. */
	TIM_CR1(TIM3) |= TIM_CR1_CEN;
    disableBrakeMoteurDroit();
    disableBrakeMoteurGauche();

}

void setConsigneTournerAvancer(coordonne consigne){
    asservConsigne = consigne;
    consigneAsservissement = TOURNERAVANCER;
    modeTournerAvancer = TOURNE;
    resetAsserv();
    /* Start timer. */
	TIM_CR1(TIM3) |= TIM_CR1_CEN;
    disableBrakeMoteurDroit();
    disableBrakeMoteurGauche();

    if((asservConsigne.y - robot.y) == 0){
        if((asservConsigne.x - robot.x)<0){
                angleTournerAvancer = -90;
            }
            else{
                angleTournerAvancer = 90;
            }
    }
    else{
        angleTournerAvancer = (atan((asservConsigne.x - robot.x)/(asservConsigne.y - robot.y))*RAD_TO_DEG);
        if((asservConsigne.y - robot.y)<0){
            if((asservConsigne.x - robot.x)<0){
                angleTournerAvancer -= 180;
            }
            else{
                angleTournerAvancer += 180;
            }
        }
        double erreurTournerAvancerProvisior = mod_angle(angleTournerAvancer-robot.alpha);
        //Gestion de la marche arrière
        // if(erreurTournerAvancerProvisior<-90 || erreurTournerAvancerProvisior>90){
        //     angleTournerAvancer = mod_angle(angleTournerAvancer+180);
        // }
    }
    saveTournerAvancer = consigne;
    asservConsigne = robot;

}

void setConsigneTourner(coordonne consigne){
    //asservConsigne.x = consigne.x;
    //asservConsigne.y = consigne.y;
    consigneAsservissement = TOURNERPOINT;
    resetAsserv();
    /* Start timer. */
	TIM_CR1(TIM3) |= TIM_CR1_CEN;
    disableBrakeMoteurDroit();
    disableBrakeMoteurGauche();

    //casParticulier a traiter
    if((consigne.y - robot.y) == 0){
        if((consigne.x - robot.x)<0){
                angleTournerPoint = -90;
            }
            else{
                angleTournerPoint = 90;
            }
    }
    else{
        angleTournerPoint = (atan((consigne.x - robot.x)/(consigne.y - robot.y))*RAD_TO_DEG);
        if((consigne.y - robot.y)<0){
            if((consigne.x - robot.x)<0){
                angleTournerPoint -= 180;
            }
            else{
                angleTournerPoint += 180;
            }
        }
        //Gestion de la marche arrière
        if(consigne.alpha!=0){
            angleTournerPoint = mod_angle(angleTournerPoint+180);
        }
    }
    //angleTournerPoint=30;
    //saveTournerPoint = consigne;
    asservConsigne = robot;

}

void stopMoteur(void){
    enableBrakeMoteurDroit();
    enableBrakeMoteurGauche();
    //stop timer
    TIM_CR1(TIM3) &= ~TIM_CR1_CEN;
    led_ON();
}


void printAsserv(void){
    // usart_send_debug("Elin : ");
    // usart_send_debugdouble(erreurPosistionLineaire);
    // usart_send_debugTab();
    // usart_send_debug("Eang : ");
    // usart_send_debugdouble(erreurPosistionAngulaire);
    // usart_send_debugTab();
    // usart_send_debug("vitLin : ");
    // usart_send_debugdouble(erreurPositionLineairePoint);
    // usart_send_debugTab();
    // usart_send_debug("VitAng : ");
    // usart_send_debugdouble(vittesseAngulaire);
    // usart_send_debugTab();
    // usart_send_debug("MD : ");
    // usart_send_debugdouble(pidVitesseLineaire-pidVitesseAngulaire);
    // usart_send_debugTab();
    // usart_send_debug("MG : ");
    // usart_send_debugdouble(pidVitesseLineaire+pidVitesseAngulaire);
}
double getinfo(void){
    return pidVitesseLineaire;
}
double getinfo2(void){
    return vitesseLineaire;
}

void asservisement_setup(void){
    nvic_enable_irq(NVIC_TIM3_IRQ);
	nvic_set_priority(NVIC_TIM3_IRQ, 64);
    rcc_periph_clock_enable(RCC_TIM3);

	/*
	 * The goal is to let the LED2 glow for a second and then be
	 * off for a second.
	 */

	/* Set timer start value. */
	TIM_CNT(TIM3) = 1;

	/* Set timer prescaler. 64MHz/640 => 50000 counts per second. */
	TIM_PSC(TIM3) = 640;

	/* End timer value. If this is reached an interrupt is generated. */
	TIM_ARR(TIM3) = 5000;

	/* Update interrupt enable. */
	TIM_DIER(TIM3) |= TIM_DIER_UIE;

	

    integralVitesseAngulaire = 0;
    integralPositionAngulaire = 0;
    integralPositionLineaire =0;
    integralVitesseLineaire = 0;

}

void tim3_isr(void){
    TIM_SR(TIM3) &= ~TIM_SR_UIF;
    Blink();
    robot = getCoordonneRobot();    

    //*********************
    //CALCUL VITESSEANGULAIRE
    //*********************
    vittesseAngulaire = (robot.alpha - positionprecedent.alpha)*FREQUENCE;
    //*********************
    //CALCUL VITESSELINEAIRE
    //*********************
    double anglevitesse;
    double longueurDeplacement = sqrt(pow(robot.y - positionprecedent.y,2)+pow(robot.x - positionprecedent.x,2))*FREQUENCE;
    if((robot.y - positionprecedent.y) == 0){
        if((robot.x - positionprecedent.x)<0){
                anglevitesse = -90;
            }
            else{
                anglevitesse = 90;
            }
    }
    else{
        anglevitesse = (atan((robot.x - positionprecedent.x)/(robot.y - positionprecedent.y))*RAD_TO_DEG);
        if((robot.y - positionprecedent.y)<0){
            if((robot.x - positionprecedent.x)<0){
                anglevitesse -= 180;
            }
            else{
                anglevitesse += 180;
            }
        }
        //erreurPositionAngulairePoint = mod_angle(angleAsserv-robot.alpha);  
    }
    //Gestion de la distance
    vitesseLineaire = longueurDeplacement*cos(mod_angle(anglevitesse-robot.alpha)*DEG_TO_RAD);

    //SAUVEGARDE
    positionprecedent = getCoordonneRobot();

    
    //*********************
    //CALCUL CORDONNEES
    //*********************
    distanceRobotPoint = sqrt(pow((asservConsigne.x - robot.x),2)+pow((asservConsigne.y - robot.y),2));
    if((asservConsigne.y - robot.y) == 0){
        if((asservConsigne.x - robot.x)<0){
                angleAsserv = -90;
            }
            else{
                angleAsserv = 90;
            }
    }
    else{
        angleAsserv = (atan((asservConsigne.x - robot.x)/(asservConsigne.y - robot.y))*RAD_TO_DEG);
        if((asservConsigne.y - robot.y)<0){
            if((asservConsigne.x - robot.x)<0){
                angleAsserv -= 180;
            }
            else{
                angleAsserv += 180;
            }
        }
        //erreurPositionAngulairePoint = mod_angle(angleAsserv-robot.alpha);  
    }
    erreurPositionAngulairePoint = mod_angle(angleAsserv-robot.alpha);
    //Gestion de la distance
    erreurPositionLineairePoint = distanceRobotPoint*cos(erreurPositionAngulairePoint*DEG_TO_RAD);

    //distance trop proche
    if(distanceRobotPoint<DISTNACEFREEZE){
        angleAsserv = freezeAngle;
    }
    else{
        freezeAngle = angleAsserv;
    }

    erreurPositionAngulairePoint = mod_angle(angleAsserv-robot.alpha);
    //Gestion de la marche arrière
    if(erreurPositionAngulairePoint<-90 || erreurPositionAngulairePoint>90){
        erreurPositionAngulairePoint = mod_angle(erreurPositionAngulairePoint+180);
    }
    
    //*********************
    //ERREUR
    //*********************
    switch (consigneAsservissement)
    {
    case AVANCER:
        asservDistance(erreurPositionLineairePoint);
        if(erreurPositionLineairePoint<DISTANCEFREINAGE && erreurPositionLineairePoint>-DISTANCEFREINAGE){
            asservFinish();
            return;
        }
        erreurPosistionAngulaire = erreurPositionAngulairePoint;
        erreurPosistionLineaire = erreurPositionLineairePoint;
        break;


    case TOURNER:
        erreurPosistionAngulaire = mod_angle(asservConsigne.alpha - robot.alpha) ;
        erreurPosistionLineaire = erreurPositionLineairePoint;
        asservDistance(erreurPosistionAngulaire);
        // if(erreurPosistionAngulaire<ANGLEFREINAGE && erreurPosistionAngulaire>-ANGLEFREINAGE){
        //     asservFinish();
        //     return;
        // }
        break;


    case TOURNERAVANCER:
        if(modeTournerAvancer == TOURNE){
            erreurPosistionAngulaire = mod_angle(angleTournerAvancer-robot.alpha);
            erreurPosistionLineaire = erreurPositionLineairePoint;
            asservDistance(erreurPosistionAngulaire);
            if(erreurPosistionAngulaire<TOURNERAVANCERCHANGE && erreurPosistionAngulaire>-TOURNERAVANCERCHANGE){
                modeTournerAvancer = AVANCE;
                asservConsigne = saveTournerAvancer;
            }
        }
        else if(modeTournerAvancer==AVANCE){
            erreurPosistionAngulaire = mod_angle(angleTournerAvancer - robot.alpha) ;
            erreurPosistionLineaire = erreurPositionLineairePoint;
            asservDistance(erreurPositionLineairePoint);
            // if(erreurPositionLineairePoint<DISTANCEFREINAGE && erreurPositionLineairePoint>-DISTANCEFREINAGE){
            //     asservFinish();
            //     return;
            // }
        }
        else{
            asservFinish();
            return;
        }
        break;


    case TOURNERPOINT:
        erreurPosistionAngulaire = mod_angle(angleTournerPoint-robot.alpha);
        erreurPosistionLineaire = erreurPositionLineairePoint;
        asservDistance(erreurPosistionAngulaire);
        // if(erreurPosistionAngulaire<TOURNERAVANCERCHANGE && erreurPosistionAngulaire>-TOURNERAVANCERCHANGE){
        //     asservFinish();
        //     return;
        // }
        break;

    
    default:
        asservFinish();
        break;
    }

    //*********************
    //ANGULAIRE
    //*********************
    //ERREUR
    //erreurPosistionAngulaire = asservConsigne.alpha - robot.alpha ;
    //Derive
    derivePositionAngulaire = oldErreurPositionAngulaire - erreurPosistionAngulaire;
    //INTEGRAL
    integralPositionAngulaire += erreurPosistionAngulaire ;
    // PID
    consignevitesseangulaire = KP_POSITIONANGULAIRE*erreurPosistionAngulaire + KI_POSITIONANGULAIRE*integralPositionAngulaire + KD_POSITIONANGULAIRE*derivePositionAngulaire;
    // Écrêtage de la vitesse
    if (consignevitesseangulaire > VITESSEMAXANGULAIRE) {
        consignevitesseangulaire = VITESSEMAXANGULAIRE;
    } 
    else if (consignevitesseangulaire < -VITESSEMAXANGULAIRE) {
        consignevitesseangulaire = -VITESSEMAXANGULAIRE;
    }
    //Écrêtage de l’accélération
    if ((consignevitesseangulaire - oldconsignevitesseangulaire) > ACCELERATIONMAXANGULAIRE) {
        consignevitesseangulaire = oldconsignevitesseangulaire + ACCELERATIONMAXANGULAIRE ;
    } 
    else if ((consignevitesseangulaire -oldconsignevitesseangulaire) < -ACCELERATIONMAXANGULAIRE) {
        consignevitesseangulaire = oldconsignevitesseangulaire - ACCELERATIONMAXANGULAIRE;
    }

    //SAUVEGARDE
    oldconsignevitesseangulaire = consignevitesseangulaire;
    oldErreurPositionAngulaire = erreurPosistionAngulaire;

    //consignevitesseangulaire = 500;

    //ERREUR
    erreurVitesseAngulaire = consignevitesseangulaire - vittesseAngulaire;
    //DERIVE
    deriveVitesseAngulaire = erreurVitesseAngulaire - oldErreurVitesseAngulaire ;
    //INTEGRAL
    integralVitesseAngulaire += erreurVitesseAngulaire;

    //PID
    if(erreurPosistionAngulaire<ANGLELIMITPRECISION && erreurPosistionAngulaire>-ANGLELIMITPRECISION){
        pidVitesseAngulaire = KP_VITESSEANGULAIRE*erreurVitesseAngulaire + KI_VITESSEANGULAIREPECISION*integralVitesseAngulaire + KD_VITESSEANGULAIRE*deriveVitesseAngulaire;
    }
    else{
        pidVitesseAngulaire = KP_VITESSEANGULAIRE*erreurVitesseAngulaire + KI_VITESSEANGULAIRE*integralVitesseAngulaire + KD_VITESSEANGULAIRE*deriveVitesseAngulaire;
    }
    
    
    
    //SAUVEGARDE
    oldErreurVitesseAngulaire = erreurVitesseAngulaire;


    //*********************
    //LINEAIRE
    //*********************

    //erreurPosistionLineaire = asservConsigne.y - robot.y ;
    //Derive
    derivePositionLineaire = oldErreurPositionLineaire - erreurPosistionLineaire;
    //INTEGRAL
    integralPositionLineaire += erreurPosistionLineaire ;
    // PID
    consigneVitesseLineaire = KP_POSITIONLINEAIRE*erreurPosistionLineaire + KI_POSITIONLINEAIRE*integralPositionLineaire + KD_POSITIONLINEAIRE*derivePositionLineaire;
    // Écrêtage de la vitesse
    if (consigneVitesseLineaire > VITESSEMAXLINEAIRE) {
        consigneVitesseLineaire = VITESSEMAXLINEAIRE;
    } 
    else if (consigneVitesseLineaire < -VITESSEMAXLINEAIRE) {
        consigneVitesseLineaire = -VITESSEMAXLINEAIRE;
    }
    //Écrêtage de l’accélération
    if ((consigneVitesseLineaire - oldConsignevitesseLineaire) > ACCELERATIONMAXLINEAIRE) {
        consigneVitesseLineaire = oldConsignevitesseLineaire + ACCELERATIONMAXLINEAIRE ;
    } 
    else if ((consigneVitesseLineaire -oldConsignevitesseLineaire) < -ACCELERATIONMAXLINEAIRE) {
        consigneVitesseLineaire = oldConsignevitesseLineaire - ACCELERATIONMAXLINEAIRE;
    }

    //SAUVEGARDE
    oldConsignevitesseLineaire = consigneVitesseLineaire;
    oldErreurPositionLineaire = erreurPosistionLineaire;



    //consigneVitesseLineaire = 500;


    //ERREUR
    erreurVitesseLineaire = consigneVitesseLineaire - vitesseLineaire;
    //DERIVE
    deriveVitesseLineaire = erreurVitesseLineaire - oldErreurVitesseLineaire;
    //INTEGRAL
    integralVitesseLineaire += erreurVitesseLineaire;

    //PID
    pidVitesseLineaire = KP_VITESSELINEAIRE*erreurVitesseLineaire + KI_VITESSELINEAIRE*integralVitesseLineaire + KD_VITESSELINEAIRE*deriveVitesseLineaire;

    //SAUVEGARDE
    oldErreurVitesseLineaire = vitesseLineaire;

//    if(pidVitesseAngulaire>100){
//        pidVitesseAngulaire = 100;
//    }
//    else if(pidVitesseAngulaire<-100){
//        pidVitesseAngulaire = -100;
//    }
   
    setVitesseMoteurDroit(pidVitesseLineaire-pidVitesseAngulaire);
    setVitesseMoteurGauche(pidVitesseLineaire+pidVitesseAngulaire);

    // setVitesseMoteurDroit(-pidVitesseAngulaire);
    // setVitesseMoteurGauche(pidVitesseAngulaire);
}


void resetAsserv(void){
    integralVitesseAngulaire = 0;
    integralPositionAngulaire = 0;
    integralVitesseLineaire = 0;
    integralPositionLineaire = 0;
    oldConsignevitesseLineaire = 0;
    oldconsignevitesseangulaire = 0;
    oldErreurPositionAngulaire = 0;
    oldErreurPositionLineaire = 0;
    oldErreurVitesseAngulaire = 0;
    oldErreurVitesseLineaire = 0;
    positionprecedent = getCoordonneRobot();
    robot = getCoordonneRobot();
}

void asservSetCallback(void (*f)(void)){
    callbackAsserv = f;
	callbackinitialiseAsserv =  true;
}

void asservFinish(void){
    enableBrakeMoteurDroit();
    enableBrakeMoteurGauche();
    //stop timer
    TIM_CR1(TIM3) &= ~TIM_CR1_CEN;
    if(callbackinitialiseAsserv == true){
        callbackAsserv();
    }
}


void asservSetDistanceCallback(void (*f)(int)){
    callbackAsservDistance = f;
	callbackinitialiseAsservDistance =  true;
}
void asservDistance(int distance){
    if(callbackinitialiseAsservDistance == true){
        callbackAsservDistance(distance);
    }
}