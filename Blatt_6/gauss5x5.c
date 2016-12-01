


<!DOCTYPE html>
 
 



<html>
  <head>
    <meta charset="utf-8" />
    <title>DHBW-Stuttgart - Shibboleth Identity Provider - Login</title>
    <link rel="stylesheet" type="text/css" href="/idp/default.css"/>
    <link rel="icon" type="image/icon" href="/idp/images/favicon.ico" />
  </head>

  <body>

          <div id="header">
                        <a href="http://www.dhbw-stuttgart.de/home.html"><img id="logo" src="/idp/images/dh-logo-h73.png" alt="DHBW Stuttgart Logo" title="DHBW Stuttgart" /></a>
                        <h1>DHBW-Stuttgart - Shibboleth Identity Provider - Login</h1>
          </div>

        <div id="wrap">

        <div id="content">

 


        


        <h2 style="break: both">Bitte geben Sie Ihre DHBW-Lehre-eMail-Adresse und Passwort ein um sich anzumelden bei:<br/>
         <font color="#880000" size="+1"><i>wwwlehre.dhbw-stuttgart.de</i></font></h2>

        <p>Um diesen Dienst zu nutzen, müssen Sie sich authentifizieren. Bitte geben Sie daher Ihre DHBW-Lehre-eMail-Adresse und Passwort ein, d.h. <i>username@lehre.dhbw-stuttgart.de</i> für Stuttgart oder <i>username@hb.dhbw-stuttgart.de</i> für Horb</p>

        <script type="text/javascript">
        <!--
        function $(id) {
          return document.getElementById(id);
        }

        function checkForm(form) {
          if( $('j_username').value.match(/@lehre.dhbw-stuttgart.de$/) 
           || $('j_username').value.match(/@hb.dhbw-stuttgart.de$/) )
                return true;

          alert("\nBitte Ihre DHBW-Lehre-eMail-Adresse eingeben, d.h.\n\n\n   username@lehre.dhbw-stuttgart.de\n\nfür Studenten/Dozenten/Mitarbeiter aus Stuttgart\n\n\noder\n\n\n   username@hb.dhbw-stuttgart.de\n\nfür Studenten/Dozenten/Mitarbeiter aus Horb\n");
          if($('j_username').value.indexOf('@') != -1)
                $('j_username').value = $('j_username').value.substring(0, $('j_username').value.indexOf("@"));
          return false;
        }

        window.onload = function() {
                if( $('j_username').value != "" && $('j_username').value.indexOf('@') == -1 ) {
                    $('j_username').value += '@lehre.dhbw-stuttgart.de';
                }
        }
        //-->
        </script>


            
              <form id="login" action="/idp/Authn/UserPassword" method="post" onsubmit="return checkForm(this);">
            

        <table>
                <tr>
                        <td rowspan="3"><img src="/idp/images/pencil.png" alt="" /></td>
                        <td style="padding: .3em;">DHBW-Lehre-eMail-Adresse</td>
                        <td><input type="text" tabindex="1" name="j_username" id="j_username" size="35" /></td>
                        <td style="padding: .4em;" rowspan="3">
                                <input type="submit" tabindex="3" value="Anmelden" />
                        </td>
                </tr>
                <tr>
                        <td style="padding: .3em;">Passwort</td>
                        <td><input type="password" tabindex="2" name="j_password" size="35" /></td>
                </tr>
		<tr>
			<td></td>
			<td><input type="checkbox" name="uApprove.consent-revocation" value="true"/> zu übermittelnde Daten erneut anzeigen</td>
		</tr>
        </table>
        </form>

            


        <hr />

        <div style="border: 1px solid #e0e0e0; background: #f0f0f0; padding: 10px">
<h2><font color=#880000>Wichtiger Hinweis zum Logout</font></h2>
<p><font color=#880000>Um sich <b>abzumelden</b>, müssen Sie den <b>WebBrowser beenden</b>, damit keine anderen Personen unter Ihrer Benutzerkennung weiterarbeiten können. Eine echte Abmeldung bei einer einzelnen Anwendung oder eine zentrale Abmeldung beim Single Sign-On sind aus technischen Gründen derzeit nicht möglich.</font></p>
        </div>

        <hr />

<h2>Hilfe, ich habe mein Passwort vergessen.</h2><p>Ohne DHBW-Lehre-eMail-Adresse und Passwort können Sie sich nicht authentifizieren und somit den Dienst nicht nutzen.</p>
<p>Kontaktieren Sie als <b>Student/Dozent/Mitarbeiter der DHBW Stuttgart</b> den <a href="http://www.dhbw-stuttgart.de/themen/service-einrichtungen/rechenzentrum/support.html">Support des Rechenzentrums</a>.<br/>Falls Sie Ihre Zugangsdaten vergessen haben, können Sie diese <b>als Student der DHBW-Stuttgart</b> <a href="https://studium.dhbw-stuttgart.de/zrz-s/passwort-reset/index.php">per Web-Interface erfahren/zurücksetzen lassen</a>.</p>
<p><b>Studenten/Dozenten/Mitarbeiter des Campus Horb</b> nehmen bitte <a href="http://www.dhbw-stuttgart.de/horb/themen/service-einrichtungen/rechenzentrum/kontakt.html">Kontakt zum Rechenzentrum Campus Horb</a> auf.</p>

<h2>Hinweis zu Bookmarks</h2>
<p>Bitte setzen Sie Ihre Bookmarks immer auf die Ziel-Webseite des Dienstes den Sie nutzen wollen, d.h. die Webseite von der aus Sie diese Anmeldeseite erreicht haben. Der direkte Aufruf dieser Anmeldeseite ohne Angabe der Ziel-Webseite funktioniert in einem Single Sign-On System nicht.</p>

<h2>Weitere Informationen</h2>
<p>Auf der <a href="http://www.dhbw-stuttgart.de/themen/service-einrichtungen/rechenzentrum/informationen-fuer-studierende/shibboleth.html" target="_blank">Informationsseite des ZRZ über Shibboleth</a> können Sie weitere Details nachlesen.</p>



        </div><!-- #content -->
        </div><!-- #wrap -->

  </body>
</html>
