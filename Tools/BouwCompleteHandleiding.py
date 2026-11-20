#!/usr/bin/env python3
"""
BouwCompleteHandleiding.py

Voegt Handleiding.md, Hardware.md en Programmauitleg.md samen tot een bestand,
Handleiding_Compleet.md, met een inhoudsopgave met links, en maakt daarvan een PDF.
Hetzelfde voor Engels: Manual.md, Hardware_EN.md en ProgramExplanation.md
worden Manual_Complete.md en een PDF.

Gebruik (in de hoofdmap van de repository, naast Handleiding.md):
    python Tools/BouwCompleteHandleiding.py            # Nederlands en Engels, md en pdf
    python Tools/BouwCompleteHandleiding.py --nl       # alleen Nederlands (of --en)
    python Tools/BouwCompleteHandleiding.py --alleen-md

De bronbestanden worden niet gewijzigd. Pas de teksten aan in de losse bestanden en draai dit script
opnieuw: dan blijven de complete bestanden gelijk.

Nodig voor de PDF: de Python-module "markdown" (pip install markdown) en Microsoft Edge.
"""
import os
import re
import subprocess
import sys
import time
import pathlib

ROOT = pathlib.Path(__file__).resolve().parent.parent
GH = "https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/"

CONFIGS = {
    "nl": dict(
        main="Handleiding.md", hard="Hardware.md", prog="Programmauitleg.md",
        out_md="Handleiding_Compleet.md", out_pdf="Handleiding_Compleet_V247.pdf", lang="nl",
        title="# Handleiding Arduino ESP32-Nano woordklok - compleet",
        subtitle="*Software WordFiboClock V247 - alle hoofdstukken in één bestand*",
        links="[English](Manual_Complete.md) · [README](READMENL.md) · [Korte gebruikershandleiding](Gebruikershandleiding.md)",
        note=("Dit bestand bevat de volledige handleiding, de hardware en bouw (hoofdstuk 3) en de programmauitleg (hoofdstuk 12). "
              "Het wordt automatisch samengesteld uit `Handleiding.md`, `Hardware.md` en `Programmauitleg.md`."),
        toc="Inhoud",
        ch3="De onderdelen, de printplaten (PCB's), de voeding, de aansluitingen en de bouw van de klokkast.",
        ch12=("De opbouw van de sketch (bestanden, klokvarianten, opslag, tijdstappen, invoer en uitvoer, WIFI, MQTT, bewaking) "
              "en hoe de software kan worden aangepast."),
        ch3_ref="hoofdstuk 3", ch12_ref="hoofdstuk 12",
        hard_anchor="3-hardware-en-bouw", prog_anchor="12-programmauitleg",
        pdf_title="Handleiding woordklok",
    ),
    "en": dict(
        main="Manual.md", hard="Hardware_EN.md", prog="ProgramExplanation.md",
        out_md="Manual_Complete.md", out_pdf="Manual_Complete_V247.pdf", lang="en",
        title="# Manual Arduino ESP32-Nano word clock - complete",
        subtitle="*Software WordFiboClock V247 - all chapters in one file*",
        links="[Nederlands](Handleiding_Compleet.md) · [README](README.md) · [Short user guide](UserGuide.md)",
        note=("This file contains the full manual, the hardware and construction (chapter 3) and the program explanation (chapter 12). "
              "It is assembled automatically from `Manual.md`, `Hardware_EN.md` and `ProgramExplanation.md`."),
        toc="Contents",
        ch3="The components, the printed circuit boards (PCBs), the power supply, the connections and the construction of the clock case.",
        ch12=("The structure of the sketch (files, clock variants, storage, time steps, input and output, WIFI, MQTT, monitoring) "
              "and how the software can be modified."),
        ch3_ref="chapter 3", ch12_ref="chapter 12",
        hard_anchor="3-hardware-and-construction", prog_anchor="12-program-explanation",
        pdf_title="Manual word clock",
    ),
}


def read(name):
    return (ROOT / name).read_text(encoding="utf8").replace("\r\n", "\n")


def slug(h):
    """GitHub-anker van een kop."""
    h = re.sub(r"<[^>]+>", "", h.strip().lower())
    h = re.sub(r"[^\w\- ]", "", h, flags=re.UNICODE)
    return h.replace(" ", "-")


def split_chapters(text):
    """Splitst op '# n. titel' (niveau 1): het stuk voor hoofdstuk 1 en de hoofdstukken."""
    parts = re.split(r"(?m)^(?=# \d+\. )", text)
    return parts[0], parts[1:]


def renumber(doc, prefix):
    """Zet '## n. titel' om naar '## prefix.n titel' en werk de ankers in de tekst bij."""
    mapping = {}
    out = []
    fence = False
    for line in doc.split("\n"):
        if line.startswith("```"):
            fence = not fence
        m = None if fence else re.match(r"^## (\d+)\. (.*)$", line)
        if m:
            old = slug(f"{m.group(1)}. {m.group(2)}")
            new_head = f"{prefix}.{m.group(1)} {m.group(2)}"
            mapping[old] = slug(new_head)
            line = f"## {new_head}"
        out.append(line)
    text = "\n".join(out)
    for old, new in mapping.items():
        text = text.replace(f"](#{old})", f"](#{new})")
    # '[16](#...)' -> '[12.16](#...)': bare nummers als linktekst
    text = re.sub(r"\[(\d+)\]\(#", lambda m: f"[{prefix}.{m.group(1)}](#", text)
    return text


def strip_front(doc):
    """Haalt titel, taalregel, inleiding en inhoudsopgave eraf (alles voor de eerste '---')."""
    first = re.search(r"(?m)^---\s*$", doc)
    body = doc[first.end():] if first else doc
    return body.strip("\n")


def localize_links(text, cfg):
    main = re.escape(cfg["main"])
    text = re.sub(r"\]\(" + main + r"#", "](#", text)
    text = text.replace("](" + cfg["main"] + ")", "](#" + slug(cfg["toc"]) + ")")
    return text


def build_md(cfg):
    out_md = ROOT / cfg["out_md"]
    hand = read(cfg["main"])
    hard = read(cfg["hard"])
    prog = read(cfg["prog"])

    head, chapters = split_chapters(hand)
    hard_body = localize_links(renumber(strip_front(hard), "3"), cfg)
    prog_body = localize_links(renumber(strip_front(prog), "12"), cfg)

    new_chapters = []
    for ch in chapters:
        n = int(re.match(r"# (\d+)\.", ch).group(1))
        if n == 3:
            title = ch.split("\n", 1)[0]
            ch = title + "\n\n" + cfg["ch3"] + "\n\n" + hard_body + "\n\n"
        elif n == 12:
            title = ch.split("\n", 1)[0]
            ch = title + "\n\n" + cfg["ch12"] + "\n\n" + prog_body + "\n\n"
        new_chapters.append(ch.rstrip("\n") + "\n")

    body = "\n".join(c if c.rstrip().endswith("---") else c + "\n---\n" for c in new_chapters)
    body = localize_links(body, cfg)
    body = body.replace("[" + cfg["hard"] + "](" + cfg["hard"] + ")", "[" + cfg["ch3_ref"] + "](#" + cfg["hard_anchor"] + ")")
    body = body.replace("[" + cfg["prog"] + "](" + cfg["prog"] + ")", "[" + cfg["ch12_ref"] + "](#" + cfg["prog_anchor"] + ")")

    # inhoudsopgave met twee niveaus
    seen = {}
    toc = []
    fence = False
    for line in body.split("\n"):
        if line.startswith("```"):
            fence = not fence
            continue
        if fence:
            continue
        m = re.match(r"^(#{1,2}) (.*)$", line)
        if m:
            a = slug(m.group(2))
            c = seen.get(a, 0)
            seen[a] = c + 1
            anchor = a if c == 0 else f"{a}-{c}"
            if m.group(1) == "#":
                toc.append(f"- **[{m.group(2)}](#{anchor})**")
            else:
                toc.append(f"  - [{m.group(2)}](#{anchor})")
    md = (cfg["title"] + "\n\n" + cfg["subtitle"] + "\n\n" + cfg["links"] + "\n\n" + cfg["note"] + "\n\n## " + cfg["toc"] + "\n\n"
          + "\n".join(toc) + "\n\n---\n\n" + body.strip("\n") + "\n")
    out_md.write_text(md, encoding="utf8", newline="\n")
    print("geschreven:", out_md, len(md.split("\n")), "regels")
    return md


def find_edge():
    for p in (r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe",
              r"C:\Program Files\Microsoft\Edge\Application\msedge.exe"):
        if os.path.exists(p):
            return p
    return None


CSS = """
@page { size: A4; margin: 15mm 14mm 17mm 14mm; @bottom-center { content: counter(page); font-size: 9pt; color: #555; } }
body { font-family: Segoe UI, Arial, sans-serif; font-size: 10pt; line-height: 1.35; color: #1b1b1b; }
h1 { font-size: 19pt; border-bottom: 2px solid #444; padding-bottom: 4px; page-break-before: always; page-break-after: avoid; }
h1:first-of-type { page-break-before: avoid; }
h2 { font-size: 13.5pt; margin-top: 15pt; border-bottom: 1px solid #bbb; padding-bottom: 2px; page-break-after: avoid; }
h3 { font-size: 11.5pt; margin-top: 11pt; page-break-after: avoid; }
table { border-collapse: collapse; width: 100%; margin: 6pt 0; font-size: 9pt; }
tr { page-break-inside: avoid; }
th, td { border: 1px solid #999; padding: 3px 5px; vertical-align: top; text-align: left; overflow-wrap: anywhere; }
th { background: #eee; }
code { font-family: Consolas, monospace; background: #f1f1f1; padding: 0 2px; font-size: 9pt; }
pre { background: #f1f1f1; padding: 5px; white-space: pre-wrap; overflow-wrap: anywhere; font-size: 8.5pt; page-break-inside: auto; }
img { max-width: 80%; max-height: 105mm; width: auto; height: auto; display: block; margin: 6pt 0; }
td img, th img { max-width: 100%; max-height: 60mm; }
a { color: #0a58b0; text-decoration: none; }
blockquote { border-left: 3px solid #999; margin: 6pt 0; padding: 0 10pt; color: #333; }
hr { display: none; }
"""


def build_pdf(md_text, cfg):
    out_pdf = ROOT / "Manual-Instructions" / cfg["out_pdf"]
    try:
        import markdown
    except ImportError:
        print("Module 'markdown' ontbreekt (pip install markdown); PDF overgeslagen.")
        return
    edge = find_edge()
    if not edge:
        print("Microsoft Edge niet gevonden; PDF overgeslagen.")
        return
    html = markdown.markdown(md_text, extensions=["tables", "fenced_code", "sane_lists"])
    seen = {}

    def add_id(m):
        level, inner = m.group(1), m.group(2)
        a = slug(re.sub(r"<[^>]+>", "", inner))
        c = seen.get(a, 0)
        seen[a] = c + 1
        ident = a if c == 0 else f"{a}-{c}"
        return f'<h{level} id="{ident}">{inner}</h{level}>'

    html = re.sub(r"<h([1-6])>(.*?)</h\1>", add_id, html, flags=re.S)
    html = re.sub(r'href="([A-Za-z0-9_\-]+\.md)(#[^"]*)?"',
                  lambda m: 'href="' + GH + m.group(1) + (m.group(2) or "") + '"', html)
    base = ROOT.as_uri() + "/"
    page = ('<!doctype html><html lang="' + cfg["lang"] + '"><head><meta charset="utf-8"><base href="' + base + '">'
            '<title>' + cfg["pdf_title"] + '</title><style>' + CSS + '</style></head><body>' + html + '</body></html>')
    tmp = ROOT / "Tools" / ("_compleet_print_" + cfg["lang"] + ".html")
    tmp.write_text(page, encoding="utf8")
    out_pdf.parent.mkdir(exist_ok=True)
    profile = ROOT / "Tools" / "_edgeprofile"
    for _ in range(3):
        if out_pdf.exists():
            out_pdf.unlink()
        subprocess.run([edge, "--headless=new", "--disable-gpu", "--no-pdf-header-footer", "--user-data-dir=" + str(profile),
                        "--virtual-time-budget=30000", "--print-to-pdf=" + str(out_pdf), tmp.as_uri()],
                       timeout=300, capture_output=True)
        for _ in range(90):
            if out_pdf.exists():
                break
            time.sleep(1)
        if out_pdf.exists():
            break
    print("pdf:", out_pdf, out_pdf.exists() and out_pdf.stat().st_size)
    for f in (tmp,):
        try:
            f.unlink()
        except OSError:
            pass


if __name__ == "__main__":
    langs = [l for l in ("nl", "en") if ("--" + l) in sys.argv] or ["nl", "en"]
    for l in langs:
        cfg = CONFIGS[l]
        text = build_md(cfg)
        if "--alleen-md" not in sys.argv:
            build_pdf(text, cfg)
