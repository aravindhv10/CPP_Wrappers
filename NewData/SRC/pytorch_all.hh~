#include "./all.hh"
#include <torch/torch.h>

namespace  TORCH_AE {

	using namespace MISC ;

	size_t constexpr
		SIZES[3] = {
			ImageResolution	*	ImageResolution	,
			24				*	24				,
			10				*	10				,
		}
	;

	size_t constexpr
		BatchSize = 200
	; //

	using TYPE_IMAGES =
		Tensors::Array::N3D_ARRAY <
			BatchSize		,
			ImageResolution	,
			ImageResolution	,
			TYPE_DATA
		>
	; //

	class Net :
	public torch::nn::Module {
	public:

		Net() {

			fc1 =
				register_module(
					"fc1",
					torch::nn::Linear(SIZES[0],SIZES[1])
				)
			; //

			fc2 =
				register_module(
					"fc2",
					torch::nn::Linear(SIZES[1],SIZES[2])
				)
			; //

			fc3 =
				register_module(
					"fc3",
					torch::nn::Linear(SIZES[2],SIZES[1])
				)
			; //

			fc4 =
				register_module(
					"fc4",
					torch::nn::Linear(SIZES[1],SIZES[0])
				)
			; //

		}

		inline torch::Tensor
		forward (
			torch::Tensor const &
				x0
		) {

			auto x1 =
				x0.reshape (
					{BatchSize,SIZES[0]}
				)
			; //

			auto x2 =
				torch::relu (
					fc1->forward(x1)
				)
			; //

			auto x3 =
				torch::relu (
					fc2->forward(x2)
				)
			; //

			auto x4 =
				torch::relu (
					fc3->forward(x3)
				)
			; //

			auto x5 =
				torch::relu (
					fc4->forward(x4)
				)
			; //

			auto x6 =
				torch::softmax (
					x5,
					/*dim=*/ 1
				)
			; //

			return
				x6
			;
		}

		torch::nn::Linear
			fc1	{nullptr}	,
			fc2	{nullptr}	,
			fc3	{nullptr}	,
			fc4	{nullptr}
		; //

	} ;

	inline void
	Train () {

		// Create a new Net.
		auto net =
			std::make_shared<Net>()
		; //

		CPPFileIO::FileLines
			ListReader (
				"./QCD_TRAIN_IMAGE_LIST"
			)
		;

		torch::optim::AdamOptions tmpopt (0.001) ;
		tmpopt.beta2(0.99) ;
		printf("DEBUG: beta2 = %e\n",tmpopt.beta2());

		torch::optim::Adam
			optimizer (
				net->parameters()	,
				tmpopt
			)
		;

		for(size_t l=0;l<4;l++)
		for(size_t j=0;j<ListReader();j++){

			CPPFileIO::FullFileReader
				<TYPE_IMAGES>
				Reader (
					ListReader (j)
				)
			;

			for(size_t k=0;k<2;k++)
			for(size_t i=0;i<Reader();i=i){

				torch::Tensor f =
					torch::from_blob (
						Reader(i).GET_DATA()	,
						{ BatchSize, SIZES[0] }
					)
				; //

				torch::Tensor prediction =
					net->forward (f)
				; //

				//std::cout << prediction << std::endl ;

				torch::Tensor loss =
					torch::mse_loss(
						prediction	,
						f
					)
				; //

				std::cout << loss ;

				loss.backward	() ;
				optimizer.step	() ;

				if ( (i%1) == 0) {

					printf (
						"Loss = %e\n"	,
						loss.item<float>()
					) ;

					torch::save (
						net			,
						"net.pt"
					) ;

				}

			}

		}

	}
}
