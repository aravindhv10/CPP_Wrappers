//STEP6_PLOTLOSSES::MakeTMVARoot();
//MXNET_CHECK::MainNet::START();
//MXNET_CHECK::MainNet::TRAIN();
//MXNET_CHECK::MainNet * slave = new MXNET_CHECK::MainNet ;
//slave->Train();
//delete slave ;

//MXNET_CHECK::MainNet slave ;
//slave.Train();

//MXNET_CHECK::MainNet::START();
//MXNET_CHECK::MainNet::TRAIN();
//printf("Back to main program...\n");

//MXNET_CHECK::AE_MXNET slave ;
//slave.Start();
//slave.BeginTrain();
//slave.MakeLoss();

if(false){
	MXNET_CHECK::AE_MXNET	slave	;
	if(true){
//		slave.Start			()	;
		slave.BeginTrain	()	;
	}
	slave.MakeLoss();
}

if(false){
	MXNET_CHECK::AE_MXNET_UNBOOSTED slave ;
	if(true){
		slave.Start();
		slave.BeginTrain();
	}
	slave.MakeLoss();
}

if(false){
	MXNET_CHECK::AE_MXNET_NOGS slave ;
	if(true){
		slave.Start();
		slave.BeginTrain();
	}
	slave.MakeLoss();
}

if(false){
	PLOT::FillQCD();
	PLOT::FillTOP();
}

if(false){
	ROC::MakeRoot_QCD_TOP_FULL();
}

if(false){
	MXNET_CNN_AE::AE_MXNET slave ;
	slave.Start();
	slave.BeginTrain();
}

if(true){
	MXNET_CHECK::MainNet slave ;
	slave.Start	() ;
	slave.Train	() ;
}
