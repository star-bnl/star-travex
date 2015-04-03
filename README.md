Regular expressions matching geometry volumes used at STAR
==========================================================

    Beam pipe                   ^.*IDSM_1/PIPI_1/PBES_1$
    Fiel cages                  ^.*TPCE_1/T[IO]FC.*$
    HFT detectors               ^.*IDSM_1/PXMO_.*$
                                ^.*IDSM_1/IBMO_.*$
                                ^.*IDSM_1/SFMO_.*$
    HFT sensitive layers        ^.*IDSM_1/PXMO_1/PXLA_[\d]+/LADR_\d/PXSI_[\d]+/PLAC_1$
                                ^.*IDSM_1/IBMO_1/IBAM_[\d]+/IBLM_\d/IBSS_1$
                                ^.*IDSM_1/SFMO_1/SFLM_[\d]+/SFSW_[\d]+/SFSL_1/SFSD_1$
    IST volumes                 ^.*IDSM_1/IBMO_1.*$
    IST sensitive volumes       ^.*IDSM_1/IBMO_1/IBAM_[\d]+/IBLM_\d/IBSS_.*$
    PXL all volumes             ^.*IDSM_1/PXMO_1.*$
    PXL sensitive volumes       ^.*IDSM_1/IBMO_1/IBAM_[\d]+/ILSB.*$
    PXL sensitive with ribs     ^.*IDSM_1/PXMO_1/PXLA_[\d]+/PXRB.*$
                                ^.*IDSM_1/PXMO_1/PXLA_[\d]+/PXLB.*$
                                ^.*IDSM_1/PXMO_1/PXLA_[\d]+/LADR_\d/PXSI_[\d]+/PLAC.*$
    SST all volumes             ^.*IDSM_1/SFMO_.*$
