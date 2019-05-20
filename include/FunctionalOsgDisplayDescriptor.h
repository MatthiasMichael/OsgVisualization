#pragma once

#include <functional>
#include <memory>

#include "Roi3DF.h"
#include "Parametrizable.h"

#include "AbstractOsgDisplayDescriptor.h"
#include "OsgDisplayable.h"


template<typename... TArgs>
class FunctionalOsgDisplayDescriptor : public AbstractOsgDisplayDescriptor
{
public:
	using DisplayableGenerator = std::function<std::unique_ptr<OsgDisplayable>(const Parametrizable &, TArgs...)>;
	using ParameterGenerator = std::function<void(Parametrizable&)>;

	FunctionalOsgDisplayDescriptor(const QString & name,
		ParameterGenerator parameterGenerator,
		DisplayableGenerator displayableGenerator,
		Roi3DF sizeHint = Roi3DF::EmptyRoi) :
		AbstractOsgDisplayDescriptor(name, sizeHint),
		m_generator(displayableGenerator)
	{
		parameterGenerator(*this);
	}

	std::unique_ptr<OsgDisplayable> makeDisplayable(TArgs... args)
	{
		return m_generator(*this, std::forward<TArgs>(args)...);
	}

private:
	DisplayableGenerator m_generator;
};


template<typename TDerived, typename... TArgs>
std::function<std::unique_ptr<OsgDisplayable>(const Parametrizable &, TArgs...)>
toPolymorphicReturnType(std::function<std::unique_ptr<TDerived>(const Parametrizable &, TArgs...)> displayableGenerator)
{
	return displayableGenerator;
}

template<typename... TArgs>
FunctionalOsgDisplayDescriptor<TArgs...> makeFunctionalOsgDisplayDescriptor_inner(
	const QString & name,
	std::function<void(Parametrizable&)> parameterGenerator,
	std::function<std::unique_ptr<OsgDisplayable>(const Parametrizable &, TArgs...)> displayableGenerator)
{
	return FunctionalOsgDisplayDescriptor<TArgs...>{name, parameterGenerator, displayableGenerator};
}

template<typename TDisplayableGenerator>
auto makeFunctionalOsgDisplayDescriptor(
	const QString & name,
	std::function<void(Parametrizable&)> parameterGenerator,
	TDisplayableGenerator displayableGenerator)
{
	return makeFunctionalOsgDisplayDescriptor_inner(name, parameterGenerator,
		toPolymorphicReturnType(std::function(displayableGenerator)));
}
