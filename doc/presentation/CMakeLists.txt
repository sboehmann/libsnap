project( presentation )

find_package( LATEX )
MACRO_LOG_FEATURE( PDFLATEX_COMPILER "PdfLaTeX" "Generate libsnap presentation" "http://www.pdftex.org" FALSE "1.40.10" "")

if( PDFLATEX_COMPILER )

  include( UseLATEX )

  file( COPY sources DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

  add_latex_document( 
      main.tex
    INPUTS
      content.tex
      einleitung.tex
      libsnap.tex
      libsnappp.tex
      csnap.tex
      snapgauge.tex
      fazit.tex
    IMAGE_DIRS
      images
    DEFAULT_PDF
  )

  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/main.pdf DESTINATION ${SNAP_DOC_INSTALL_DIR} RENAME presentation.pdf )
endif( PDFLATEX_COMPILER )