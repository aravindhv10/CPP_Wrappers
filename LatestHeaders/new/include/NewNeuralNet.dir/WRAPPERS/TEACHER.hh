#ifndef _HEADER_GUARD_NewNeuralNet_WRAPPERS_TEACHER_
#define _HEADER_GUARD_NewNeuralNet_WRAPPERS_TEACHER_

#include "../../StaticArray.hh"
#include "../TEACHER.hh"

#include "./headers_start.hh"

namespace THREADED_NN {

	template <
		size_t S_T , size_t		S_B ,
		size_t S_Y , typename	TF	= float
	> class L2Teacher {
	public:

		inline static size_t constexpr
		SIZE_X () {return S_Y;}

		_MACRO__DEFINE_0_

		using TYPE_NET =
			NN::L2Teacher <
				SIZE_B()	,
				SIZE_Y()	,
				TYPE_DATA
			>
		; //

		using TYPE_NETS =
			StaticArray::ND_ARRAY <
				SIZE_T()	,
				TYPE_NET
			>
		; //

		_MACRO_INHERIT_(TYPE_I1)
		_MACRO_INHERIT_(TYPE_I2)
		_MACRO_INHERIT_(TYPE_O1)

		using TYPE_BACKWARD_INPUT =
			struct {
				size_t	const	i	;
				TYPE_I1	const &	I1	;
				TYPE_I2	const &	I2	;
			}
		; //

		using TYPE_BACKWARD_OUTPUT =
			struct {
				size_t	const	i	;
				TYPE_O1	const &	O1	;
			}
		; //

		inline TYPE_BACKWARD_OUTPUT const
		BACKWARD(
			TYPE_BACKWARD_INPUT const
				in
		) {

			NETS[in.i].I1 =
				& in.I1
			; //

			NETS[in.i].I2 =
				& in.I2
			; //

			NETS[in.i]
				.BACKWARD()
			; //

			TYPE_BACKWARD_OUTPUT ret =
				{in.i,NETS[in.i].O1}
			; //

			return
				ret
			; //

		}

		TYPE_NETS NETS ;

	} ;

}

namespace THREADED_NN_GRAD {

	template <
		size_t S_T , size_t S_B ,
		size_t S_Z , size_t S_Y ,
		typename TF	= float
	> class L2Teacher {
	public:

		inline static size_t constexpr
		SIZE_X () {return S_Y;}

		_MACRO__DEFINE_0_B_

		using TYPE_NET =
			NN_GRAD::L2Teacher <
				SIZE_B()	,
				SIZE_Z()	,
				SIZE_Y()	,
				TYPE_DATA
			>
		; //

		using TYPE_NETS =
			StaticArray::ND_ARRAY <
				SIZE_T()	,
				TYPE_NET
			>
		; //

		_MACRO_INHERIT_(TYPE_I1)
		_MACRO_INHERIT_(TYPE_I2)
		_MACRO_INHERIT_(TYPE_ID1)
		_MACRO_INHERIT_(TYPE_ID2)
		_MACRO_INHERIT_(TYPE_O1)
		_MACRO_INHERIT_(TYPE_OD1)

		using TYPE_BACKWARD_INPUT =
			struct {
				size_t		const	i	;
				TYPE_I1		const &	I1	;
				TYPE_I2		const &	I2	;
				TYPE_ID1	const &	ID1	;
				TYPE_ID2	const &	ID2	;
			}
		; //

		using TYPE_BACKWARD_OUTPUT =
			struct {
				size_t		const	i	;
				TYPE_O1		const &	O1	;
				TYPE_OD1	const &	OD1	;
			}
		; //

		inline TYPE_BACKWARD_OUTPUT const
		BACKWARD (
			TYPE_BACKWARD_INPUT const
				in
		) {

			NETS[in.i].I1 =
				& in.I1
			; //

			NETS[in.i].ID1 =
				& in.ID1
			; //

			NETS[in.i].I2 =
				& in.I2
			; //

			NETS[in.i].ID2 =
				& in.ID2
			; //

			NETS[in.i]
				.BACKWARD()
			; //

			TYPE_BACKWARD_OUTPUT ret =
				{
					in.i			,
					NETS[in.i].O1	,
					NETS[in.i].OD1
				}
			; //

			return
				ret
			; //

		}

		TYPE_NETS NETS ;

	} ;

}

#include "./headers_end.hh"

#endif
