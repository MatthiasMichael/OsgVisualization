#pragma once

#include <any>
#include <cassert>
#include <memory>
#include <typeinfo>
#include <vector>

#include <QObject>
#include <QString>

#include "Roi3DF.h"

#include "OsgDisplayable.h"
#include "AbstractOsgDisplayDescriptor.h"
#include <typeindex>


class TypeErasedOsgDisplayDescriptor
{
public:
	template
	<
		typename TDisplayDescriptor,
		typename = std::enable_if_t<std::is_base_of_v<AbstractOsgDisplayDescriptor, std::decay_t<TDisplayDescriptor>>>
	>
	/*implicit*/ TypeErasedOsgDisplayDescriptor(TDisplayDescriptor && descriptor) :
		m_containedDescriptor(make_container(std::forward<TDisplayDescriptor>(descriptor),
		                                     &std::remove_reference_t<TDisplayDescriptor>::makeDisplayable))
	{
		// empty
	}

	~TypeErasedOsgDisplayDescriptor() = default;

	TypeErasedOsgDisplayDescriptor(const TypeErasedOsgDisplayDescriptor &) = delete;
	TypeErasedOsgDisplayDescriptor & operator=(const TypeErasedOsgDisplayDescriptor &) = delete;

	TypeErasedOsgDisplayDescriptor(TypeErasedOsgDisplayDescriptor && other) noexcept : 
		m_containedDescriptor(std::move(other.m_containedDescriptor))
	{
		// empty
	}

	TypeErasedOsgDisplayDescriptor & operator=(TypeErasedOsgDisplayDescriptor && other) noexcept
	{
		m_containedDescriptor = std::move(other.m_containedDescriptor);
		return *this;
	}

	AbstractOsgDisplayDescriptor & getPolymorphicDescriptor()
	{
		return m_containedDescriptor->getPolymorphicDescriptor();
	}

	template<typename TDescriptor>
	TDescriptor & getConcreteDescriptor()
	{
		if(std::type_index(typeid(TDescriptor)) != std::type_index(m_containedDescriptor->type_info()))
		{
			throw std::bad_cast{};
		}

		return static_cast<Container<TDescriptor>&>(*m_containedDescriptor).m_descriptor;
	}

	std::unique_ptr<OsgDisplayable> makeDisplayable(std::vector<std::any> & arguments) const
	{
		return m_containedDescriptor->invoke_makeDisplayable(arguments);
	}

	void notifyOnAnyParameterChange(QObject* receiver, const char * slot) const
	{
		m_containedDescriptor->notifyOnAnyParameterChange(receiver, slot);
	}

	void setSizeHint(Roi3DF area) { m_containedDescriptor->setSizeHint(area); }

	const QString & getName() const { return m_containedDescriptor->getName(); }
	Roi3DF getSizeHint() const { return m_containedDescriptor->getSizeHint(); }

private:
	struct PolymorphicContainer
	{
		virtual ~PolymorphicContainer() = default;

		virtual const std::type_info & type_info() const = 0;

		virtual AbstractOsgDisplayDescriptor & getPolymorphicDescriptor() = 0;
		virtual std::unique_ptr<OsgDisplayable> invoke_makeDisplayable(const std::vector<std::any> & arguments) = 0;

		virtual void notifyOnAnyParameterChange(QObject* receiver, const char * slot) const = 0;

		virtual void setSizeHint(Roi3DF area) = 0;

		virtual const QString & getName() const = 0;
		virtual Roi3DF getSizeHint() const = 0;
	};


	template <typename TDisplayDescriptor, typename... TArgs>
	struct Container : PolymorphicContainer
	{
		TDisplayDescriptor m_descriptor;

		template <typename T>
		explicit Container(T && value) : m_descriptor(std::forward<T>(value)) { }

		const std::type_info & type_info() const override { return typeid(TDisplayDescriptor); }

		AbstractOsgDisplayDescriptor & getPolymorphicDescriptor() override
		{
			return m_descriptor;
		}

		std::unique_ptr<OsgDisplayable> invoke_makeDisplayable(const std::vector<std::any> & arguments) override
		{
			assert(arguments.size() == sizeof...(TArgs));
			return invoke_makeDisplayable(arguments, std::make_index_sequence<sizeof...(TArgs)>());
		}

		template <size_t... Indices>
		std::unique_ptr<OsgDisplayable> invoke_makeDisplayable(const std::vector<std::any> & arguments,
		                                                       std::index_sequence<Indices...>)
		{
			return m_descriptor.makeDisplayable(std::any_cast<TArgs>(arguments[Indices])...);
		}

		void notifyOnAnyParameterChange(QObject * receiver, const char * slot) const override
		{
			m_descriptor.notifyOnAnyParameterChange(receiver, slot);
		}

		void setSizeHint(Roi3DF area) override { m_descriptor.setSizeHint(area); }

		const QString & getName() const override { return m_descriptor.getName(); }
		Roi3DF getSizeHint() const override { return m_descriptor.getSizeHint(); }
	};

	template <typename TDisplayDescriptor, typename... TArgs>
	std::unique_ptr<PolymorphicContainer> make_container(
		TDisplayDescriptor && descriptor,
		std::unique_ptr<OsgDisplayable> (std::remove_reference_t<TDisplayDescriptor>::*)(TArgs...))
	{
		return std::make_unique<Container<TDisplayDescriptor, TArgs...>>(std::forward<TDisplayDescriptor>(descriptor));
	}

	std::unique_ptr<PolymorphicContainer> m_containedDescriptor;
};
