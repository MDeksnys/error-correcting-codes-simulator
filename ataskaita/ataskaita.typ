#set text(lang: "lt")
#show link: it => underline(text(fill: blue)[#it])
#show figure.caption: set text(size: 0.8em)
#show figure.caption: emph

#align(center + horizon)[
  #set text(1.5em)
  #text(2em)[
    *Užduoties A11 ataskaita*\
  ]
  Martynas Deksnys
]

#pagebreak()
#outline()
#pagebreak()

= Įgyvendintos užduoties dalys

- 1, 2 ir 3 scenarijai.
- Tam tikri veiksmai su vektoriais ir matricomis, sudarytais iš baigtinio
  kūno $FF_2$ elementų.
- Kodavimas tiesiniu kodu $C[n,k]$.
- Siuntimas kanalu.
- Dekodavimas, naudojant grandinio (step-by-step) dekodavimo algoritmą.
- Teksto (paveiksliuko) skaidymas reikiamo ilgio vektoriais iš kūno $FF_2$ elementų
  ir šių vektorių sujungimas atgal į tekstą (paveiksliuką).

= Trečiųjų šalių funkcijų bibliotekos

Nėra.

= Programos paleidimas

Paleidžiamasis failas: *A11* (Linux) _arba_ *A11.exe* (Windows).

#figure(
  ```bash
  # Linux
  gcc -O3 -DNDEBUG -fopenmp src/*.c -o A11
  # Windows
  cl /O2 /DNDEBUG /openmp:llvm src\*.c /FeA11.exe /link Shell32.lib
  ```,
  caption: [Programos kompiliavimas pavyzdys]
)

#figure(
  ```bash
  # Linux
  ./A11 -p 0.0001 -n 3 -k 2 -g 101,011 -s 3 --seed 123
  # Windows
  A11.exe -p 0,1 -n 3 -k 2 -g 101011
  ```,
  caption: [Programos paleidimo pavyzdys]
)

Būtini parametrai:
- Klaidos tikimybė: `-p [tikimybė]`.
  - `[tikimybė]` tai skaičius tarp 0 ir 1 (imtinai).
  - Veikia su tašku ir su kableliu. T.y. veikia ir $0.1$, ir $0,1$.
- Kodo ilgis: `-n [skaičius]`.
- Kodo dimensija: `-k [skaičius]`.
- Kodą generuojanti matrica: `-g [matrica]`.
  - `[matrica]` tai $n dot m$ ilgio 0-ių ir 1-tų seka,
    kurią galima suskaidyti naudojant kablelį "`,`".

Pasirinktini parametrai:
- Scenarijus: `-s [scenarijus]`.
  - `[scenarijus]` tai viena iš šitų reikšmių: 1, 2, 3.
  - Nepateikus bus einami visi scenarijai iš eilės.
- Atsitiktinių skaičių generatoriaus sėkla: `--seed [skaičius]`.
  - Nepateikus naudojamas dabartinis laikas.

= Programos failai

- src/bit_string.c
  - Funkcijos darbui su bitu eilutėmis: konvertavimas į baitų eilutes, skaidymas į
    vektorius ir pan.
- src/channel.c
  - Funkcijos kanalo modeliui:
    klaidų vektorių generavimas ir vektorių persiuntimas.
- src/cli.c
  - Komandinės eilutės argumentų apdorojimas.
- src/decode.c
  - Grandininio dekodavimo algoritmo realizacija.
- src/encode.c
  - Kodavimas tiesiniu kodu.
- src/main.c
  - Programos įėjimo taškas.
- src/matrix.c
  - Matricų operacijos ir pagalbinės funkcijos.
- src/memory.c
  - Pagalbinės atminties valdymo funkcijos.
- src/scenario1.c
  - Pirmo scenarijaus logika.
- src/scenario2.c
  - Antro scenarijaus logika.
- src/scenario3.c
  - Trečio scenarijaus logika.
- src/vector.c
  - Vektorių operacijos ir pagalbinės funkcijos.

`[pavadinimas]_internal.h` failuose deklaruojamos tik to paties modulio
(`[pavadinimas].c`) vidinės pagalbinės funkcijos. Šie `.h` failai nenaudojami
kituose moduliuose.
Kiti `.h` failai yra standartiniai ir skirti bendram naudojimui.

= Naudotojo sąsaja

Paleidus programa be visų būtinų argumentų, arba pateikus netinkamus argumentus,
vartotojui apie tai pranešama.

Po to pagal scenarijų:
+ Vartotojo prašoma pateikti $n$ ilgio dvejetainį vektorių
  - Jei pateikiamas per ilgas vektorius, paemami pirmi $n$ simboliai.
  - Jei pateikiamas per trumpas vektorius arba jei pateikto vektoriaus pirmuose
    $n$ simboliuose yra netinkamas simbolis (ne "0" ir ne "1"), vartotojo prašoma
    įvesti vektorių iš naujo.

  Vartotojui parodomas atsitiktinis klaidos vektorius (iš kanalo) ir paklausiama,
  ar jis nori įvesti klaidos vektorių pats. Įvedimo tikrinimas analogiškas ankstesniam.

  Vartotojui parodomas dekoduotas vektorius ir jo originaliai įvestas vektorius
  palyginimui.

+ Vartotojo prašoma pateikti tekstą.
  - Tekstą galima rašyti keliomis eilutėmis.
  - Teksto įvedimas užbaigiamas, kai
    - pasiekiamas 1024 simbolių limitas, arba
    - vartotojas tuščioje eilutėje įveda tašką ("`.`") ir paspaudžia Enter.

  Vartotojui parodomi trys tekstai:
  + Originaliai įvestas tekstas.
  + Tekstas siustas pro kanalą (nekoduotas).
  + Tekstas siustas pro kanalą (koduotas ir dekoduotas).

+ Vartotojo prašoma pateikti BMP failo pavadinimą.
  - Jei įvedamas netinkamas failo pavadinimas (neegzistuojantis failas arba
    be "`.bmp`" galūnės), vartotojui pranešama ir prašoma bandyti iš naujo.

  Atidaromas pradinis BMP failas.

  Kol vyksta siuntimas ir kiti veiksmai, vartotojui pranešama, kokį veiksmą programa
  tuo metu atlikinėja: "Sending over channel...", "Decoding..." ir t.t.

  Atidaromi 2 nauji BMP failai:
  + Siustas pro kanalą (nekoduotas).
  + Siustas pro kanalą (koduotas ir dekoduotas).

= Padaryti programiniai sprendimai

Jei tekstą suskaidžius vektoriais negaunamas pilnas vektorius, prie vektoriaus galo
pridedami 0-iai tol kol ilgis teisingas. Pridėtų 0-ių skaičius įsimenamas, kad
vėliau dekoduojant vektorių, jų galima būtų atsikratyti.

Vektoriai siunčiami kanalu prie jų pridedant atsitiktinai sugeneruotą klaidų vektorių.

Vektoriai saugomi kaip simbolių masyvai (`char *`), t.y. kiekvienas duomenų bitas saugomas kaip
'0' arba '1'. Tai aštuonis kartus padidina atminties naudojimą ir apsunkina tam tikrus
skaičiavimus, bet alternatyvūs laikymo metodai taip pat turi savų sudėtingumų.

= Eksperimentai

#grid(
  columns: (1fr, 1fr),
  rows: 1,
  align(horizon)[
  Tyrinėjama, kaip nuo kanalo klaidos tikimybės priklauso skirtingų kodų
  sėkmingo dekodavimo tikimybės.

  Dekodavimas laikytas sėkmingu tik tuo atveju, jei dekoduota žinutė identiška
  originaliai.

  Testuoti du kodai: *$[7, 4]$ Hemingo kodas* ir *pakartojimo kodas $R(5)$*.
  Testuotos tikimybės nuo 0 iki 1, intervalas 0.05.
  Su kiekviena tikimybe kiekvienas kodas buvo testuotas $1000$ kartų.

  $[7, 4]$ Hemingo kodo sėkmingo dekodavimo tikimybės priklausomybė
  nuo kanalo klaidos tikimybės pavaizduota @H74.

  Kodą generuojanti matrica:

  $
    G=mat(
      1, 0, 0, 0, 1, 0, 1;
      0, 1, 0, 0, 1, 1, 0;
      0, 0, 1, 0, 1, 1, 1;
      0, 0, 0, 1, 0, 1, 1;
    )
  $

  Pakartojimo kodo $R(5)$ sėkmingo dekodavimo tikimybės priklausomybė
  nuo kanalo klaidos tikimybės pavaizduota @R5.

  Kodą generuojanti matrica:

  $
    G=mat(
      1, 1, 1, 1, 1;
    )
  $
  ],
  [
    #figure(
      image("grafikas1.svg"),
      caption: [$[7, 4]$ Hemingo kodas]
    ) <H74>
    #figure(
      image("grafikas2.svg"),
      caption: [Pakartojimo kodas $R(5)$]
    ) <R5>
  ]
)

= Naudota literatūra

- Klaidas taisančių kodų teorija. Paskaitų konspektai.
- [VO89] S.A.Vanstone, P.C. van Oorschot. An introduction to error correcting codes with
  applications. Kluwer Academic Publishers, Boston, 1989.
- #link("https://en.wikipedia.org/wiki/BMP_file_format")[BMP formato vikipedija]
