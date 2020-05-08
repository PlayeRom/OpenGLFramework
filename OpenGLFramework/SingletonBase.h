/*! \file
	\brief Plik nag³ówkowy dla klasy CSingletonBase.
*/

#pragma once

/// Klasa szablonu singletonów
/** Wszelkie klasy maj¹ce byæ singletonami powinny dziedziczyæ po tej klasie. */
template<typename T> class CSingletonBase
{
public:
	/// Destruktor
	/** Destruktor */
	virtual ~CSingletonBase() { m_pInst.release(); } 

	/// Statyczna metoda za pomoc¹, której pobieramy wskaŸnik na obiekt klasy szablonu. 
	/*! \return WskaŸnik na obiekt klasy szablonu. Je¿eli nie wywo³amy wczeœniej Construct to metoda
	GetInstance zwróci \p NULL! */
	static inline T* GetInstance() { return m_pInst.get(); }

protected:
	/// Statyczna metoda tworz¹ca obiekt klasy szablonu. 
	/** \return WskaŸnik na obiekt klasy szablonu. */
	static inline T* Construct( T *pInst ) {
		if( !m_pInst.get() )
			m_pInst.reset( pInst );
		return m_pInst.get();
	}

private:
	static std::auto_ptr<T> m_pInst; ///< Instancja klasy.
};

template<typename T> std::auto_ptr<T> CSingletonBase<T>::m_pInst;