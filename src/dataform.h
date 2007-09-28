/*
  Copyright (c) 2005-2007 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef DATAFORM_H__
#define DATAFORM_H__

#include "dataformbase.h"
#include "stanzaextension.h"

#include <string>
#include <list>

namespace gloox
{

  class Tag;

  namespace DataForm
  {

    class Item;
    class Reported;

    /**
     * Describes the possible Form Types.
     */
    enum FormType
    {
      TypeForm,                     /**< The forms-processing entity is asking the forms-submitting
                                     * entity to complete a form. */
      TypeSubmit,                   /**< The forms-submitting entity is submitting data to the
                                     * forms-processing entity. */
      TypeCancel,                   /**< The forms-submitting entity has cancelled submission of data
                                     * to the forms-processing entity. */
      TypeResult,                   /**< The forms-processing entity is returning data (e.g., search
                                     * results) to the forms-submitting entity, or the data is a
                                     * generic data set. */
      TypeInvalid                   /**< The form is invalid. Only possible if the form was created
                                     * from an Tag which doesn't correctly describe a Data Form. */
    };

    /**
     * @brief An abstraction of a XEP-0004 Data Form.
     *
     *
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 0.7
     */
    class GLOOX_API FormBase : public StanzaExtension, public FieldContainer
    {
      public:
        /**
         * Constructs a new, empty form.
         * @param type The form type.
         * @param instructions Natural-language instructions for filling out the form. Should not contain
         * newlines (\\n, \\r).
         * @param title The natural-language title of the form. Should not contain newlines (\\n, \\r).
         */
        FormBase( FormType type, const StringList& instructions, const std::string& title = "" );

        /**
         * Constructs a new, empty form without any instructions or title set. Probably best suited for
         * result forms.
         * @param type The form type.
         * @param title The natural-language title of the form. Should not contain newlines (\\n, \\r).
         * @since 0.9
         */
        FormBase( FormType type, const std::string& title = "" );

        /**
         * Constructs a new DataForm from an existing Tag/XML representation.
         * @param tag The existing form to parse.
         */
        FormBase( const Tag* tag );

        /**
         * Virtual destructor.
         */
        virtual ~FormBase();

        /**
         * Use this function to retrieve the title of the form.
         * @return The title of the form.
         */
        const std::string& title() const { return m_title; }

        /**
         * Use this function to set the title of the form.
         * @param title The new title of the form.
         * @note The title should not contain newlines (\\n, \\r).
         */
        void setTitle( const std::string& title ) { m_title = title; }

        /**
         * Retrieves the natural-language instructions for the form.
         * @return The fill-in instructions for the form.
         */
        const StringList& instructions() const { return m_instructions; }

        /**
         * Use this function to set natural-language instructions for the form.
         * @param instructions The instructions for the form.
         * @note The instructions should not contain newlines (\\n, \\r). Instead, every line should be an
         * element of the StringMap. This allows for platform dependent newline handling on the target
         * platform.
         */
        void setInstructions( const StringList& instructions ) { m_instructions = instructions; }

        /**
         * Returns the form's type.
         * @return The form's type.
         * @since 0.9
         */
        FormType type() const { return m_type; }

        /**
         * Parses the given Tag and creates an appropriate DataForm representation.
         * @param tag The Tag to parse.
         * @return @b True on success, @b false otherwise.
         * @since 0.9
         */
        bool parse( const Tag* tag );

        /**
         * Converts to  @b true if the FormBase is valid, @b false otherwise.
         */
        operator bool() const { return m_type != TypeInvalid; }

        // reimplemented from StanzaExtension
        virtual const std::string filterString() const
        {
          return "/message/x[@xmlns='" + XMLNS_X_DATA + "']";
        }

        // reimplemented from StanzaExtension
        virtual StanzaExtension* newInstance( const Tag* tag ) const
        {
          return new FormBase( tag );
        }

        // reimplemented from StanzaExtension
        virtual Tag* tag() const;

      protected:
        FormType m_type;

      private:
        StringList m_instructions;

        std::string m_title;

    };

    /**
     * @brief A DataForm of type 'form'.
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class GLOOX_API Form : public FormBase
    {
      public:
        /**
         *
         */
        Form( const StringList& instructions, const std::string& title = "" )
          : FormBase( TypeForm, instructions, title ) {}

        /**
         *
         */
        Form()
          : FormBase( TypeCancel ) {}

    };

    /**
     * @brief A DataForm of type 'submit'.
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class GLOOX_API Submit : public FormBase
    {
      public:
        /**
         *
         */
        Submit( const StringList& instructions, const std::string& title = "" )
          : FormBase( TypeSubmit, instructions, title ) {}

        /**
         *
         */
        Submit()
          : FormBase( TypeSubmit ) {}

    };

    /**
     * @brief A DataForm of type 'cancel'.
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class GLOOX_API Cancel : public FormBase
    {
      public:
        /**
         *
         */
        Cancel( const StringList& instructions, const std::string& title = "" )
          : FormBase( TypeCancel, instructions, title ) {}

        /**
         *
         */
        Cancel()
          : FormBase( TypeCancel ) {}

    };

    /**
     * @brief A DataForm of type 'result'.
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class GLOOX_API Result : public FormBase
    {
      public:

        /**
         *
         */
        typedef std::list<Item*> ItemList;

        /**
         *
         */
        Result( const StringList& instructions, const std::string& title = "" )
          : FormBase( TypeResult, instructions, title ) {}

        /**
         *
         */
        Result( const Tag* tag );

        /**
         *
         */
        Result()
          : FormBase( TypeResult ) {}

      private:
        Reported* m_reported;
        ItemList m_items;
    };

  }

}

#endif // DATAFORM_H__
