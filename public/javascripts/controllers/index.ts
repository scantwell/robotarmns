/// <reference path='../_app.ts' />

/**
 * Created by omnia on 11/16/15.
 */

module RobotArm {
    export class IndexController {
        public status: string = 'boot';

        //noinspection JSUnusedGlobalSymbols
        public static $inject = ['$mdSidenav', '$http', '$mdDialog', '$timeout'];

        constructor(private mdSideNav: any,
                    private http: ng.IHttpService,
                    private mdDialog: any,
                    private timeout: ng.ITimeoutService) {
            this.status = "connected"
        }

        public toggleSidePane() {
            this.mdSideNav('left')
                .toggle()
                .then(function () {
                    //console.log("Toggled Left is Done");
                });
        }

        public moveForward() {
            /* WIP - Loading Overaly
            this.mdDialog.show({
                templateUrl: '/templates/loadingOverlay.html',
                parent: angular.element(document.body),
                clickOutsideToClose: false
            });
            this.timeout(() => {
                this.mdDialog.hide();
            }, 1000);
            */

            this.http.post('/api/moveforward', {value: 50}
            ).then((success) => {
                this.timeout(() => {
                    this.mdDialog.hide();
                });
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public turnLeft() {
            this.http.post('/api/turnleft', {value: 50}
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public turnRight() {
            this.http.post('/api/turnright', {value: 50}
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public moveBackward() {
            this.http.post('/api/movebackward', {value: 50}
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public openClaw() {
            this.http.post('/api/openclaw', {value: 50}
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public closeClaw() {
            this.http.post('/api/closeclaw', {value: 50}
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public raiseArm() {
            this.http.post('/api/raisearm', {value: 50}
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public lowerArm() {
            this.http.post('/api/lowerarm', {value: 50}
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
    }

    angular.module('RobotArm').controller('IndexController', IndexController);
}