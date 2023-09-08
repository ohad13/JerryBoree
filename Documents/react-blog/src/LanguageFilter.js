import React from 'react';
import { useHistory, useLocation } from 'react-router-dom';
import { useState } from "react";
import { useEffect } from "react";

const LanguageFilter = () => {
  const history = useHistory();
  const location = useLocation();
  const queryParams = new URLSearchParams(location.search);

  // Initialize selectedLanguage state based on query parameter
  const [selectedLanguage, setSelectedLanguage] = useState(
    queryParams.get('language') || ''
  );

  useEffect(() => {
    // Handle reset when no language parameter is present
    if (!queryParams.has('language')) {
      setSelectedLanguage('');
    }
  }, [queryParams]);

  const handleLanguageChange = event => {
    const selectedLanguage = event.target.value;
    setSelectedLanguage(selectedLanguage);

    // Update URL with the selected language
    history.push({ search: selectedLanguage ? `?language=${selectedLanguage}` : '' });
  };

  return (
    <div>
      <label>Filter by Language: </label>
      <select onChange={handleLanguageChange}>
        <option value="">All Languages</option>
        <option value="en">English</option>
        <option value="es">Spanish</option>
        <option value="fr">French</option>
        <option value="de">German</option>
        <option value="it">Italian</option>
        <option value="pt">Portuguese</option>
        <option value="zh">Chinese</option>
        <option value="ja">Japanese</option>
        <option value="ko">Korean</option>
        <option value="ru">Russian</option>
        <option value="ar">Arabic</option>        
      </select>
    </div>
  );
};

export default LanguageFilter;
