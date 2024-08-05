const introText = document.getElementById('intro-text');
const fullText = document.getElementById('full-text');
const btnToggle = document.getElementById('btn-toggle');

btnToggle.addEventListener('click', () => {
    if (introText.classList.contains('hidden')) {
        introText.classList.remove('hidden');
        fullText.classList.add('hidden');
        btnToggle.textContent = "Lanjutkan Membaca";
    } else {
        introText.classList.add('hidden');
        fullText.classList.remove('hidden');
        btnToggle.textContent = "Tampilkan Ringkasan";
    }
});